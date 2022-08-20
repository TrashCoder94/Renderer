#include <Shared/Renderer.h>
#include <Shared/RendererAPI.h>

#if PLATFORM_WINDOWS
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW
#include <cassert>
#include <cstdint>

#include <DirectX12/DirectX12CommandParameters.h>

void ParseCommandLineArguments(RendererCommandParameters* pCommandParameters)
{
    int argc;
    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
    DirectX12CommandParameters* pDX12CommandParameters = static_cast<DirectX12CommandParameters*>(pCommandParameters);
    assert(pDX12CommandParameters && "DirectX12 command parameters has not been created yet!");

    for (size_t i = 0; i < argc; ++i)
    {
        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0)
        {
            pDX12CommandParameters->SetWindowWidth(::wcstol(argv[++i], nullptr, 800));
        }
        if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0)
        {
            pDX12CommandParameters->SetWindowHeight(::wcstol(argv[++i], nullptr, 600));
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
    // Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
    // Using this awareness context allows the client area of the window 
    // to achieve 100% scaling while still allowing non-client window content to 
    // be rendered in a DPI sensitive fashion.
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Parsing command line arguments and storing them in our class for the Renderer when we initialize
    RendererCommandParameters* pCommandParameters = RendererCommandParameters::Create();
    const wchar_t* windowClassName = L"DX12WindowClass";
    ParseCommandLineArguments(pCommandParameters);

    Renderer::Initialize(pCommandParameters);
    {
        Renderer::Update();
    }
    Renderer::Deinitialize();

    //Renderer::Initialize();
    //{
    //    if (RendererAPI* const pRendererAPI = Renderer::GetRendererAPI())
    //    {
    //        constexpr float buildMachineTestTime{ 5.0f };
    //        float currentTime{ 0.0f };

    //        // Render loop
    //        float lastTime{ pRendererAPI->GetTime() };
    //        while (Renderer::IsRunning())
    //        {
    //            const float time{ pRendererAPI->GetTime() };
    //            const float deltaTime{ time - lastTime };
    //            lastTime = time;
    //            Renderer::Update(deltaTime);

    //            currentTime += deltaTime;
    //            if (currentTime >= buildMachineTestTime)
    //            {
    //                Renderer::SetRunning(false);
    //                break;
    //            }
    //        }
    //    }
    //}
    //Renderer::Deinitialize();

	return 0;
}
#endif