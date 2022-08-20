#pragma once

#include "rpch.h"

#if PLATFORM_WINDOWS
#include <DirectX12/DirectX12CommandParameters.h> \

#define IMPLEMENT_APPLICATION(x) \
    HINSTANCE g_HInstance; \
    void ParseCommandLineArguments(RendererCommandParameters* pCommandParameters) \
    { \
        int argc; \
        wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc); \
        DirectX12CommandParameters* pDX12CommandParameters = static_cast<DirectX12CommandParameters*>(pCommandParameters); \
        assert(pDX12CommandParameters && "DirectX12 command parameters has not been created yet!"); \
        pDX12CommandParameters->SetWindowName(#x); \
        for (size_t i = 0; i < argc; ++i) \
        { \
            if (::wcscmp(argv[i], L"-width") == 0 || ::wcscmp(argv[i], L"--width") == 0) \
            { \
                pDX12CommandParameters->SetWindowWidth(::wcstol(argv[++i], nullptr, 10)); \
            } \
            if (::wcscmp(argv[i], L"-height") == 0 || ::wcscmp(argv[i], L"--height") == 0) \
            { \
                pDX12CommandParameters->SetWindowHeight(::wcstol(argv[++i], nullptr, 10)); \
            } \
            if (::wcscmp(argv[i], L"-fullscreen") == 0 || ::wcscmp(argv[i], L"--fullscreen") == 0) \
            { \
                pDX12CommandParameters->SetFullscreen(true); \
            } \
            if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0) \
            { \
                pDX12CommandParameters->SetUseWarp(true); \
            } \
        } \
        ::LocalFree(argv); \
    } \
    int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
    { \
        g_HInstance = hInstance; \
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2); \
        RendererAPI::API selectedAPI = RendererAPI::GetAPI(); \
        int argc; \
        wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc); \
        for (size_t i = 0; i < argc; ++i) \
        { \
            if (::wcscmp(argv[i], L"-api") == 0 || ::wcscmp(argv[i], L"--api") == 0) \
            { \
                wchar_t* apiArg = argv[++i]; \
                if (::wcscmp(argv[i], L"DX12") == 0 || ::wcscmp(argv[i], L"DirectX12") == 0 || ::wcscmp(argv[i], L"directx12") == 0) \
                { \
                    selectedAPI = RendererAPI::API::DirectX12; \
                } \
                else if (::wcscmp(argv[i], L"Vulkan") == 0 || ::wcscmp(argv[i], L"vulkan") == 0) \
                { \
                    selectedAPI = RendererAPI::API::Vulkan; \
                } \
            } \
        } \
        RendererCommandParameters* pCommandParameters = RendererCommandParameters::Create(selectedAPI); \
        ParseCommandLineArguments(pCommandParameters); \
        Renderer::Initialize(pCommandParameters); \
        { \
            Renderer::Update(); \
        } \
        Renderer::Deinitialize(); \
        return 0; \
    }
#endif