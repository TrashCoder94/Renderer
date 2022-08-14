#include <Shared/Renderer.h>
#include <Shared/RendererAPI.h>

int main(int argc, char** argv)
{
    Renderer::Initialize();
    {
        if (RendererAPI* const pRendererAPI = Renderer::GetRendererAPI())
        {
            // Render loop
            float lastTime{ pRendererAPI->GetTime() };
            while (Renderer::IsRunning())
            {
                const float time{ pRendererAPI->GetTime() };
                const float deltaTime{ time - lastTime };
                lastTime = time;
                Renderer::Update(deltaTime);
            }
        }
    }
    Renderer::Deinitialize();

	return 0;
}