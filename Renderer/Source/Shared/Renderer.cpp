#include "Renderer.h"
#include "Shared/RendererAPI.h"
#include "Shared/RendererCommandParameters.h"
#include "Shared/Window.h"

#include <chrono>
#include <iostream>

RendererAPI* Renderer::s_pRendererAPI = nullptr;
Window* Renderer::s_pWindow = nullptr;
float Renderer::s_DeltaTime = 0.016f;
bool Renderer::s_Running = true;

void Renderer::Initialize(RendererCommandParameters* pCommandParameters)
{
	s_pRendererAPI = RendererAPI::Create();
    s_pWindow = Window::Create("Learn DX12", pCommandParameters->GetWindowWidth(), pCommandParameters->GetWindowHeight());

	s_pRendererAPI->Initialize(pCommandParameters);
}

void Renderer::Update()
{
    static uint64_t frameCounter = 0;
    static float elapsedSeconds = 0.0f;
    static std::chrono::high_resolution_clock clock;
    static auto previousTime = clock.now();

    s_Running = true;

    while (s_Running)
    {
        frameCounter++;
        auto currentTime = clock.now();
        auto deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        s_DeltaTime = deltaTime.count() * 1e-9f;
        
        if (s_pRendererAPI->IsInitialized())
        {
            s_Running = s_pRendererAPI->Update(s_DeltaTime);
            s_pRendererAPI->Render();

            elapsedSeconds += s_DeltaTime;
            if (elapsedSeconds > 1.0f)
            {
                auto fps = frameCounter / elapsedSeconds;
                std::cout << "FPS: " << fps << std::endl;

                frameCounter = 0;
                elapsedSeconds = 0.0f;
            }
        }
    }
}

void Renderer::Deinitialize()
{
	s_pRendererAPI->Deinitialize();
	s_pWindow->Deinitialize();

    delete s_pRendererAPI;
    s_pRendererAPI = nullptr;

    delete s_pWindow;
    s_pWindow = nullptr;
}

RendererAPI* const Renderer::GetRendererAPI()
{
    return s_pRendererAPI;
}

Window* const Renderer::GetWindow()
{
    return s_pWindow;
}

void Renderer::SetRunning(const bool running)
{
    s_Running = running;
}

const bool Renderer::IsRunning()
{
    return s_Running;
}

const float Renderer::GetDeltaTime()
{
    return s_DeltaTime;
}
