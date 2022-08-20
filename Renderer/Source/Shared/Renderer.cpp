#include "rpch.h"
#include "Renderer.h"

std::shared_ptr<RendererAPI> Renderer::s_pRendererAPI = nullptr;
std::shared_ptr<Window> Renderer::s_pWindow = nullptr;
float Renderer::s_DeltaTime = 0.016f;
float Renderer::s_AutoQuitTime = 0.0f;
bool Renderer::s_Running = true;

void Renderer::Initialize(std::shared_ptr<RendererCommandParameters> pCommandParameters)
{
	s_pRendererAPI = RendererAPI::Create(pCommandParameters->GetAPI());
    s_pWindow = Window::Create(pCommandParameters->GetWindowName(), pCommandParameters->GetWindowWidth(), pCommandParameters->GetWindowHeight());

	s_pRendererAPI->Initialize(pCommandParameters);
}

void Renderer::Update()
{
    static uint64_t frameCounter = 0;
    static float fpsSeconds = 0.0f;
    static std::chrono::high_resolution_clock clock;
    static auto previousTime = clock.now();
    static bool checkForAutoQuitTime = s_AutoQuitTime > 0.0f;
    static float elapsedSeconds = 0.0f;

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
            fpsSeconds += s_DeltaTime;

            if (checkForAutoQuitTime)
            {
                if (elapsedSeconds >= s_AutoQuitTime)
                {
                    std::cout << elapsedSeconds << "s have passed so Renderer will stop updating now." << std::endl;
                    break;
                }
            }

            if (fpsSeconds > 1.0f)
            {
                auto fps = frameCounter / fpsSeconds;
                std::cout << "FPS: " << fps << std::endl;

                frameCounter = 0;
                fpsSeconds = 0.0f;
            }
        }
    }
}

void Renderer::Deinitialize()
{
	s_pRendererAPI->Deinitialize();
	s_pWindow->Deinitialize();
}

std::shared_ptr<RendererAPI>& Renderer::GetRendererAPI()
{
    return s_pRendererAPI;
}

std::shared_ptr<Window>& Renderer::GetWindow()
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

void Renderer::SetAutoQuitTime(const float autoQuitTime)
{
    s_AutoQuitTime = autoQuitTime;
}
