#include "rpch.h"
#include "DirectX12RendererAPI.h"

DirectX12RendererAPI::DirectX12RendererAPI() : RendererAPI(),
	m_UseWarp(false),
	m_Msg(),
	m_pDX12Window(nullptr),
	m_pDevice(nullptr),
	m_pCommandQueue(nullptr),
	m_pSwapChain(nullptr),
	m_pBackBuffers(),
	m_pCommandList(nullptr),
	m_pCommandAllocators(),
	m_pRTVDescriptorHeap(nullptr),
	m_RTVDescriptorSize(0),
	m_CurrentBackBufferIndex(0),
	m_pFence(nullptr),
	m_FenceValue(0),
	m_FrameFenceValues(),
	m_FenceEvent()
{}

DirectX12RendererAPI::~DirectX12RendererAPI()
{}

void DirectX12RendererAPI::Initialize(RendererCommandParameters* pCommandParameters)
{
	// Always make sure this is called first in Initialize
	EnableDebugLayer();

	DirectX12Window* pDX12Window = Renderer::GetWindow<DirectX12Window>();
	assert(pDX12Window && "DX12 window has not been created yet!");
	m_pDX12Window = pDX12Window;
	m_pDX12Window->SetTearingSupported(CheckTearingSupport());

	DirectX12CommandParameters* pDX12CommandParameters = static_cast<DirectX12CommandParameters*>(pCommandParameters);
	m_pDX12Window->Initialize(pCommandParameters);

	m_UseWarp = pDX12CommandParameters->GetUseWarp();
	ComPtr<IDXGIAdapter4> pDxgiAdapter4 = GetAdapter(m_UseWarp);

	m_pDevice = CreateDevice(pDxgiAdapter4);

	m_pCommandQueue = CreateCommandQueue(m_pDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);

	m_pSwapChain = CreateSwapChain(m_pDX12Window->m_HWND, m_pCommandQueue, m_pDX12Window->GetWidth(), m_pDX12Window->GetHeight(), s_NumFrames);

	m_CurrentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	m_pRTVDescriptorHeap = CreateDescriptorHeap(m_pDevice, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, s_NumFrames);
	m_RTVDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	UpdateRenderTargetViews(m_pDevice, m_pSwapChain, m_pRTVDescriptorHeap);

	for (int i = 0; i < s_NumFrames; ++i)
	{
		m_pCommandAllocators[i] = CreateCommandAllocator(m_pDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	m_pCommandList = CreateCommandList(m_pDevice, m_pCommandAllocators[m_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

	m_pFence = CreateFence(m_pDevice);
	m_FenceEvent = CreateEventHandle();

	m_IsInitialized = true;
	
	m_pDX12Window->Show();
	m_Msg = {};
}

void DirectX12RendererAPI::Render()
{
	auto pCommandAllocator = m_pCommandAllocators[m_CurrentBackBufferIndex];
	auto pBackBuffer = m_pBackBuffers[m_CurrentBackBufferIndex];

	pCommandAllocator->Reset();
	m_pCommandList->Reset(pCommandAllocator.Get(), nullptr);

	// Clear the render target.
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			pBackBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		m_pCommandList->ResourceBarrier(1, &barrier);

		FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_pRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_CurrentBackBufferIndex, m_RTVDescriptorSize);
		m_pCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	}

	// Present
	{
		CD3DX12_RESOURCE_BARRIER pBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			pBackBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_pCommandList->ResourceBarrier(1, &pBarrier);

		ThrowIfFailed(m_pCommandList->Close());

		ID3D12CommandList* const pCommandLists[] = {
			m_pCommandList.Get()
		};
		m_pCommandQueue->ExecuteCommandLists(_countof(pCommandLists), pCommandLists);

		if (!m_pDX12Window)
		{
			if (Window* pWindow = Renderer::GetWindow())
			{
				if (DirectX12Window* pDX12Window = static_cast<DirectX12Window*>(pWindow))
				{
					m_pDX12Window = pDX12Window;
					m_pDX12Window->SetTearingSupported(CheckTearingSupport());
				}
			}
		}
		assert(m_pDX12Window && "DirectX12Window wasn't initialized!");

		UINT syncInterval = m_pDX12Window->IsVSync() ? 1 : 0;
		UINT presentFlags = (m_pDX12Window->IsTearingSupported() && !m_pDX12Window->IsVSync()) ? DXGI_PRESENT_ALLOW_TEARING : 0;
		ThrowIfFailed(m_pSwapChain->Present(syncInterval, presentFlags));

		m_FrameFenceValues[m_CurrentBackBufferIndex] = Signal(m_pCommandQueue, m_pFence, m_FenceValue);

		m_CurrentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		WaitForFenceValue(m_pFence, m_FrameFenceValues[m_CurrentBackBufferIndex], m_FenceEvent);
	}
}

bool DirectX12RendererAPI::Update(const float deltaTime)
{
	if (::PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&m_Msg);
		::DispatchMessage(&m_Msg);
	}

	return m_Msg.message != WM_QUIT;
}

void DirectX12RendererAPI::Deinitialize()
{
	// Make sure the command queue has finished all commands before closing.
	Flush(m_pCommandQueue, m_pFence, m_FenceValue, m_FenceEvent);

	::CloseHandle(m_FenceEvent);
}

void DirectX12RendererAPI::ResizeWindow(uint32_t width, uint32_t height)
{
	assert(m_pDX12Window && "DirectX12Window has not been initialized!");

	if (m_pDX12Window->GetWidth() != width || m_pDX12Window->GetHeight() != height)
	{
		// Don't allow 0 size swap chain back buffers.
		m_pDX12Window->SetWidth(std::max(1u, width));
		m_pDX12Window->SetHeight(std::max(1u, height));

		const uint32_t windowWidth = m_pDX12Window->GetWidth();
		const uint32_t windowHeight = m_pDX12Window->GetHeight();

		// Flush the GPU queue to make sure the swap chain's back buffers
		// are not being referenced by an in-flight command list.
		Flush(m_pCommandQueue, m_pFence, m_FenceValue, m_FenceEvent);

		for (int i = 0; i < s_NumFrames; ++i)
		{
			// Any references to the back buffers must be released
			// before the swap chain can be resized.
			m_pBackBuffers[i].Reset();
			m_FrameFenceValues[i] = m_FrameFenceValues[m_CurrentBackBufferIndex];
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		ThrowIfFailed(m_pSwapChain->GetDesc(&swapChainDesc));
		ThrowIfFailed(m_pSwapChain->ResizeBuffers(s_NumFrames, windowWidth, windowHeight, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));

		m_CurrentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		UpdateRenderTargetViews(m_pDevice, m_pSwapChain, m_pRTVDescriptorHeap);
	}
}

void DirectX12RendererAPI::EnableDebugLayer()
{
#if defined(DEBUG)
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer.
	ComPtr<ID3D12Debug> pDebugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugInterface)));
	pDebugInterface->EnableDebugLayer();
#endif
}

ComPtr<IDXGIAdapter4> DirectX12RendererAPI::GetAdapter(bool useWarp)
{
	ComPtr<IDXGIFactory4> pDxgiFactory;
	UINT createFactoryFlags = 0;
#if defined(DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&pDxgiFactory)));

	ComPtr<IDXGIAdapter1> pDxgiAdapter1;
	ComPtr<IDXGIAdapter4> pDxgiAdapter4;

	if (useWarp)
	{
		ThrowIfFailed(pDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pDxgiAdapter1)));
		ThrowIfFailed(pDxgiAdapter1.As(&pDxgiAdapter4));
	}
	else
	{
		SIZE_T maxDedicatedVideoMemory = 0;
		for (UINT i = 0; pDxgiFactory->EnumAdapters1(i, &pDxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			pDxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

			// Check to see if the adapter can create a D3D12 device without actually 
			// creating it. The adapter with the largest dedicated video memory
			// is favored.
			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED(D3D12CreateDevice(pDxgiAdapter1.Get(),
					D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
			{
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				ThrowIfFailed(pDxgiAdapter1.As(&pDxgiAdapter4));
			}
		}
	}

	return pDxgiAdapter4;
}

ComPtr<ID3D12Device2> DirectX12RendererAPI::CreateDevice(ComPtr<IDXGIAdapter4> pAdapter)
{
	ComPtr<ID3D12Device2> pD3d12Device2;
	ThrowIfFailed(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pD3d12Device2)));

	// Enable debug messages in debug mode.
#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> pInfoQueue;
	if (SUCCEEDED(pD3d12Device2.As(&pInfoQueue)))
	{
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

		// Suppress whole categories of messages
		//D3D12_MESSAGE_CATEGORY Categories[] = {};

		// Suppress messages based on their severity level
		D3D12_MESSAGE_SEVERITY Severities[] =
		{
			D3D12_MESSAGE_SEVERITY_INFO
		};

		// Suppress individual messages by their ID
		D3D12_MESSAGE_ID DenyIds[] = {
			/*
				See: https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
				Workaround for Windows 11
			*/
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE,

			/*
				This warning occurs when a render target is cleared using a clear color 
				that is not the optimized clear color specified during resource creation. 
				If you want to clear a render target using an arbitrary clear color, 
				you should disable this warning.
			*/
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
			
			/*
				These warnings occur when a frame is captured using the graphics debugger 
				integrated in Visual Studio. Since I think this bug will never be fixed in 
				the debugger, it’s best to just ignore this warning.
			*/
			D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
			D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
		};

		D3D12_INFO_QUEUE_FILTER NewFilter = {};
		//NewFilter.DenyList.NumCategories = _countof(Categories);
		//NewFilter.DenyList.pCategoryList = Categories;
		NewFilter.DenyList.NumSeverities = _countof(Severities);
		NewFilter.DenyList.pSeverityList = Severities;
		NewFilter.DenyList.NumIDs = _countof(DenyIds);
		NewFilter.DenyList.pIDList = DenyIds;

		ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
	}
#endif

	return pD3d12Device2;
}

ComPtr<ID3D12CommandQueue> DirectX12RendererAPI::CreateCommandQueue(ComPtr<ID3D12Device2> pDevice, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12CommandQueue> pD3d12CommandQueue;

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pD3d12CommandQueue)));

	return pD3d12CommandQueue;
}

bool DirectX12RendererAPI::CheckTearingSupport()
{
	BOOL allowTearing = FALSE;

	// Rather than create the DXGI 1.5 factory interface directly, we create the
	// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
	// graphics debugging tools which will not support the 1.5 factory interface 
	// until a future update.
	ComPtr<IDXGIFactory4> pFactory4;
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&pFactory4))))
	{
		ComPtr<IDXGIFactory5> pFactory5;
		if (SUCCEEDED(pFactory4.As(&pFactory5)))
		{
			if (FAILED(pFactory5->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing, sizeof(allowTearing))))
			{
				allowTearing = FALSE;
			}
		}
	}

	return allowTearing == TRUE;
}

ComPtr<IDXGISwapChain4> DirectX12RendererAPI::CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> pCommandQueue, uint32_t width, uint32_t height, uint32_t bufferCount)
{
	ComPtr<IDXGISwapChain4> pDxgiSwapChain4;
	ComPtr<IDXGIFactory4> pDxgiFactory4;
	UINT createFactoryFlags = 0;
#if defined(DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&pDxgiFactory4)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	// It is recommended to always allow tearing if tearing support is available.
	swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	ComPtr<IDXGISwapChain1> pSwapChain1;
	ThrowIfFailed(pDxgiFactory4->CreateSwapChainForHwnd(
		pCommandQueue.Get(),
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&pSwapChain1));

	// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
	// will be handled manually.
	ThrowIfFailed(pDxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(pSwapChain1.As(&pDxgiSwapChain4));

	return pDxgiSwapChain4;
}

ComPtr<ID3D12DescriptorHeap> DirectX12RendererAPI::CreateDescriptorHeap(ComPtr<ID3D12Device2> pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors)
{
	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = type;

	ThrowIfFailed(pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pDescriptorHeap)));

	return pDescriptorHeap;
}

void DirectX12RendererAPI::UpdateRenderTargetViews(ComPtr<ID3D12Device2> pDevice, ComPtr<IDXGISwapChain4> pSwapChain, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap)
{
	auto rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < s_NumFrames; ++i)
	{
		ComPtr<ID3D12Resource> pBackBuffer;
		ThrowIfFailed(pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer)));

		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, rtvHandle);

		m_pBackBuffers[i] = pBackBuffer;

		rtvHandle.Offset(rtvDescriptorSize);
	}
}

ComPtr<ID3D12CommandAllocator> DirectX12RendererAPI::CreateCommandAllocator(ComPtr<ID3D12Device2> pDevice, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12CommandAllocator> pCommandAllocator;
	ThrowIfFailed(pDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&pCommandAllocator)));
	return pCommandAllocator;
}

ComPtr<ID3D12GraphicsCommandList> DirectX12RendererAPI::CreateCommandList(ComPtr<ID3D12Device2> pDevice, ComPtr<ID3D12CommandAllocator> pCommandAllocator, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12GraphicsCommandList> pCommandList;
	ThrowIfFailed(pDevice->CreateCommandList(0, type, pCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&pCommandList)));
	ThrowIfFailed(pCommandList->Close());
	return pCommandList;
}

ComPtr<ID3D12Fence> DirectX12RendererAPI::CreateFence(ComPtr<ID3D12Device2> pDevice)
{
	ComPtr<ID3D12Fence> pFence;
	ThrowIfFailed(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence)));
	return pFence;
}

HANDLE DirectX12RendererAPI::CreateEventHandle()
{
	HANDLE fenceEvent;
	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent && "Failed to create fence event.");
	return fenceEvent;
}

uint64_t DirectX12RendererAPI::Signal(ComPtr<ID3D12CommandQueue> pCommandQueue, ComPtr<ID3D12Fence> pFence, uint64_t& fenceValue)
{
	uint64_t fenceValueForSignal = ++fenceValue;
	ThrowIfFailed(pCommandQueue->Signal(pFence.Get(), fenceValueForSignal));
	return fenceValueForSignal;
}

void DirectX12RendererAPI::WaitForFenceValue(ComPtr<ID3D12Fence> pFence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration)
{
	if (pFence->GetCompletedValue() < fenceValue)
	{
		ThrowIfFailed(pFence->SetEventOnCompletion(fenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
	}
}

void DirectX12RendererAPI::Flush(ComPtr<ID3D12CommandQueue> pCommandQueue, ComPtr<ID3D12Fence> pFence, uint64_t& fenceValue, HANDLE fenceEvent)
{
	uint64_t fenceValueForSignal = Signal(pCommandQueue, pFence, fenceValue);
	WaitForFenceValue(pFence, fenceValueForSignal, fenceEvent);
}