#include <Shared/Renderer.h>
#include <Shared/RendererAPI.h>

int main(int argc, char** argv)
{
    Renderer::Initialize();
    {
        if (RendererAPI* const pRendererAPI = Renderer::GetRendererAPI())
        {
            constexpr float buildMachineTestTime{ 5.0f };
            float currentTime{ 0.0f };

            // Render loop
            float lastTime{ pRendererAPI->GetTime() };
            while (Renderer::IsRunning())
            {
                const float time{ pRendererAPI->GetTime() };
                const float deltaTime{ time - lastTime };
                lastTime = time;
                Renderer::Update(deltaTime);

                currentTime += deltaTime;
                if (currentTime >= buildMachineTestTime)
                {
                    Renderer::SetRunning(false);
                    break;
                }
            }
        }
    }
    Renderer::Deinitialize();

	return 0;
}