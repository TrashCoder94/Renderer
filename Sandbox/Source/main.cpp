#include <Shared/Renderer.h>
#include <Shared/RendererAPI.h>

#if PLATFORM_WINDOWS
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW
#include <cassert>
#include <cstdint>

#include <DirectX12/DirectX12CommandParameters.h>

HINSTANCE g_HInstance;

void ParseCommandLineArguments(RendererCommandParameters* pCommandParameters)
{
    int argc;
    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
    DirectX12CommandParameters* pDX12CommandParameters = static_cast<DirectX12CommandParameters*>(pCommandParameters);
    assert(pDX12CommandParameters && "DirectX12 command parameters has not been created yet!");

    for (size_t i = 0; i < argc; ++i)
    {
        if (::wcscmp(argv[i], L"-width") == 0 || ::wcscmp(argv[i], L"--width") == 0)
        {
            pDX12CommandParameters->SetWindowWidth(::wcstol(argv[++i], nullptr, 10));
        }
        if (::wcscmp(argv[i], L"-height") == 0 || ::wcscmp(argv[i], L"--height") == 0)
        {
            pDX12CommandParameters->SetWindowHeight(::wcstol(argv[++i], nullptr, 10));
        }
        if (::wcscmp(argv[i], L"-fullscreen") == 0 || ::wcscmp(argv[i], L"--fullscreen") == 0)
        {
            pDX12CommandParameters->SetFullscreen(true);
        }
        if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0)
        {
            pDX12CommandParameters->SetUseWarp(true);
        }
    }

    // Free memory allocated by CommandLineToArgvW
    ::LocalFree(argv);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_HInstance = hInstance;

    // Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
    // Using this awareness context allows the client area of the window 
    // to achieve 100% scaling while still allowing non-client window content to 
    // be rendered in a DPI sensitive fashion.
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Parsing command line arguments and storing them in our class for the Renderer when we initialize
    RendererCommandParameters* pCommandParameters = RendererCommandParameters::Create();
    ParseCommandLineArguments(pCommandParameters);

    Renderer::Initialize(pCommandParameters);
    {
        Renderer::Update();
    }
    Renderer::Deinitialize();

    return 0;
}
#endif