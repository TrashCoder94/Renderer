#pragma once

class RendererAPI;
class Window;

class Renderer
{
	public:
		static void Initialize();
		static void Update(const float deltaTime);
		static void Deinitialize();

		static RendererAPI* const GetRendererAPI();
		static Window* const GetWindow();

		static void SetRunning(const bool running);
		static const bool IsRunning();

		static const float GetDeltaTime();

	private:
		static RendererAPI* s_pRendererAPI;
		static Window* s_pWindow;
		static float s_DeltaTime;
		static bool s_Running;
};