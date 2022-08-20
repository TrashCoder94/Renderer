#include "DirectX12Window.h"
#include "DirectX12/DirectX12.h"
#include "DirectX12/DirectX12RendererAPI.h"
#include "DirectX12/DirectX12CommandParameters.h"
#include "Shared/Renderer.h"
#include "Shared/RendererCommandParameters.h"

#include <cassert>

extern HINSTANCE g_HInstance;

DirectX12Window::DirectX12Window(const std::string& name, const uint32_t width, const uint32_t height) : Window(name, width, height),
	m_HWND(),
	m_WindowRect(),
	m_TearingSupported(false)
{}

DirectX12Window::~DirectX12Window()
{}

void DirectX12Window::Initialize(RendererCommandParameters* pCommandParameters)
{
	std::wstring windowNameWString = std::wstring(m_Name.begin(), m_Name.end());
	const wchar_t* windowNameWChar = windowNameWString.c_str();
	RegisterWindowClass(g_HInstance, windowNameWChar);
	CreateWindow(windowNameWChar, g_HInstance, windowNameWChar, m_Width, m_Height, m_HWND);

	// Initialize the global window rect variable.
	::GetWindowRect(m_HWND, &m_WindowRect);

	m_Fullscreen = pCommandParameters->IsFullscreen();
}

void DirectX12Window::Deinitialize()
{

}

void DirectX12Window::Show()
{
	if (m_Fullscreen)
	{
		SetFullscreen(true);
	}
	else
	{
		::ShowWindow(m_HWND, SW_SHOW);
	}
}

void DirectX12Window::Hide()
{
	::ShowWindow(m_HWND, SW_HIDE);
}

void DirectX12Window::SetFullscreen(bool enabled)
{
	m_Fullscreen = enabled;

	if (m_Fullscreen) // Switching to fullscreen.
	{
		// Store the current window dimensions so they can be restored 
		// when switching out of fullscreen state.
		::GetWindowRect(m_HWND, &m_WindowRect);

		// Set the window style to a borderless window so the client area fills
		// the entire screen.
		UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

		::SetWindowLongW(m_HWND, GWL_STYLE, windowStyle);

		// Query the name of the nearest display device for the window.
		// This is required to set the fullscreen dimensions of the window
		// when using a multi-monitor setup.
		HMONITOR hMonitor = ::MonitorFromWindow(m_HWND, MONITOR_DEFAULTTONEAREST);
		MONITORINFOEX monitorInfo = {};
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		::GetMonitorInfo(hMonitor, &monitorInfo);

		::SetWindowPos(m_HWND, HWND_TOP,
			monitorInfo.rcMonitor.left,
			monitorInfo.rcMonitor.top,
			monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
			monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		::ShowWindow(m_HWND, SW_MAXIMIZE);
	}
	else
	{
		// Restore all the window decorators.
		::SetWindowLong(m_HWND, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		::SetWindowPos(m_HWND, HWND_NOTOPMOST,
			m_WindowRect.left,
			m_WindowRect.top,
			m_WindowRect.right - m_WindowRect.left,
			m_WindowRect.bottom - m_WindowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		::ShowWindow(m_HWND, SW_NORMAL);
	}
}

LRESULT DirectX12Window::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DirectX12Window::RegisterWindowClass(const HINSTANCE& hInst, const wchar_t* windowClassName)
{
    // Register a window class for creating our render window with.
    WNDCLASSEXW windowClass = {};

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = &WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInst;
    windowClass.hIcon = ::LoadIcon(hInst, NULL);
    windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = windowClassName;
    windowClass.hIconSm = ::LoadIcon(hInst, NULL);

    static ATOM atom = ::RegisterClassExW(&windowClass);
    assert(atom > 0);
}

void DirectX12Window::CreateWindow(const wchar_t* windowClassName, const HINSTANCE& hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height, HWND& outHWND)
{
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// Center the window within the screen. Clamp to 0, 0 for the top-left corner.
	int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
	int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

	outHWND = ::CreateWindowExW(
		NULL,
		windowClassName,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		NULL,
		NULL,
		hInst,
		nullptr
	);

	assert(outHWND && "Failed to create window");
}

void DirectX12Window::SetTearingSupported(const bool tearingSupported)
{
	m_TearingSupported = tearingSupported;
}

bool DirectX12Window::IsTearingSupported() const
{
	return m_TearingSupported;
}

LRESULT DirectX12Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RendererAPI* pAPI = Renderer::GetRendererAPI();
	DirectX12RendererAPI* pDX12API = static_cast<DirectX12RendererAPI*>(pAPI);
	assert(pDX12API && "DirectX12RendererAPI hasn't been initialized!");

	Window* pWindow = Renderer::GetWindow();
	DirectX12Window* pDX12Window = static_cast<DirectX12Window*>(pWindow);
	assert(pDX12Window && "DirectX12Window hasn't been initialized!");

	if (pDX12API->IsInitialized())
	{
		switch (uMsg)
		{
			case WM_PAINT:
			{
				//Renderer::Update();
				/*pDX12API->Update(Renderer::GetDeltaTime());
				pDX12API->Render();*/
				break;
			}
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

				switch (wParam)
				{
					case 'V':
					{
						pDX12Window->SetVSync(!pDX12Window->IsVSync());
						break;
					}
					case VK_ESCAPE:
					{
						::PostQuitMessage(0);
						break;
					}
					case VK_RETURN:
					case VK_F11:
					{
						if (alt)
						{
							pDX12Window->SetFullscreen(!pDX12Window->IsFullscreen());
						}
						break;
					}
				}

				break;
			}
			// The default window procedure will play a system notification sound 
			// when pressing the Alt+Enter keyboard combination if this message is 
			// not handled.
			case WM_SYSCHAR:
			{
				break;
			}
			case WM_SIZE:
			{
				RECT clientRect = {};
				::GetClientRect(pDX12Window->m_HWND, &clientRect);

				int width = clientRect.right - clientRect.left;
				int height = clientRect.bottom - clientRect.top;

				pDX12API->ResizeWindow(width, height);
				break;
			}
			// Check if the window is being destroyed.
			case WM_DESTROY:
			{
				::PostQuitMessage(0);
				return 0;
			}
			// Check if the window is being closed.
			case WM_CLOSE:
			{
				::PostQuitMessage(0);
				return 0;
			}
			default:
			{
				return MessageHandler(hwnd, uMsg, wParam, lParam);
			}
		}
	}

	return MessageHandler(hwnd, uMsg, wParam, lParam);
}