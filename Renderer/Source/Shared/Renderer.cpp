#include "Renderer.h"
#include "Shared/RendererAPI.h"
#include "Shared/Window.h"

RendererAPI* Renderer::s_pRendererAPI = nullptr;
Window* Renderer::s_pWindow = nullptr;
float Renderer::s_DeltaTime = 0.016f;
bool Renderer::s_Running = true;

void Renderer::Initialize()
{
	s_pRendererAPI = RendererAPI::Create();
	s_pRendererAPI->Initialize();

	s_pWindow = Window::Create("Learn OpenGL", 800, 600);
	s_pWindow->Initialize();
}

void Renderer::Update(const float deltaTime)
{
    s_DeltaTime = deltaTime;
    s_Running = s_pWindow->Update(s_DeltaTime);
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
