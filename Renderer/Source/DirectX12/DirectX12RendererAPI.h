#pragma once

#include "Shared/RendererAPI.h"

class DirectX12Window;

class DirectX12RendererAPI : public RendererAPI
{
public:
	DirectX12RendererAPI();
	~DirectX12RendererAPI();

	virtual void Initialize(RendererCommandParameters* pCommandParameters) override;
	virtual void Render() override;
	virtual bool Update(const float deltaTime) override;
	virtual void Deinitialize() override;

	bool CheckTearingSupport();
	void ResizeWindow(uint32_t width, uint32_t height);

private:
	void EnableDebugLayer();
	ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp);
	ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> pAdapter);
	ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> pDevice, D3D12_COMMAND_LIST_TYPE type);
	ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> pCommandQueue, uint32_t width, uint32_t height, uint32_t bufferCount);
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);
	void UpdateRenderTargetViews(ComPtr<ID3D12Device2> pDevice, ComPtr<IDXGISwapChain4> pSwapChain, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap);
	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> pDevice, D3D12_COMMAND_LIST_TYPE type);
	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> pDevice, ComPtr<ID3D12CommandAllocator> pCommandAllocator, D3D12_COMMAND_LIST_TYPE type);
	ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> pDevice);
	HANDLE CreateEventHandle();
	uint64_t Signal(ComPtr<ID3D12CommandQueue> pCommandQueue, ComPtr<ID3D12Fence> pFence, uint64_t& fenceValue);
	void WaitForFenceValue(ComPtr<ID3D12Fence> pFence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration = std::chrono::milliseconds::max());
	void Flush(ComPtr<ID3D12CommandQueue> pCommandQueue, ComPtr<ID3D12Fence> pFence, uint64_t& fenceValue, HANDLE fenceEvent);

	static constexpr uint8_t s_NumFrames = 3; // The number of swap chain back buffers.
	
	bool m_UseWarp; // Use WARP adapter
	
	MSG m_Msg;
	DirectX12Window* m_pDX12Window;

	// DirectX 12 Objects
	ComPtr<ID3D12Device2> m_pDevice;
	ComPtr<ID3D12CommandQueue> m_pCommandQueue;
	ComPtr<IDXGISwapChain4> m_pSwapChain;
	ComPtr<ID3D12Resource> m_pBackBuffers[s_NumFrames];
	ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
	ComPtr<ID3D12CommandAllocator> m_pCommandAllocators[s_NumFrames];
	ComPtr<ID3D12DescriptorHeap> m_pRTVDescriptorHeap;
	UINT m_RTVDescriptorSize;
	UINT m_CurrentBackBufferIndex;

	// Synchronization objects
	ComPtr<ID3D12Fence> m_pFence;
	uint64_t m_FenceValue = 0;
	uint64_t m_FrameFenceValues[s_NumFrames] = {};
	HANDLE m_FenceEvent;
};