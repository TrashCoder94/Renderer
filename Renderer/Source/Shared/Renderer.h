#pragma once

class RendererCommandParameters;
class RendererAPI;
class Window;

class Renderer
{
	public:
		static void Initialize(RendererCommandParameters* pCommandParameters);
		static void Update();
		static void Deinitialize();

		static RendererAPI* const GetRendererAPI();
		static Window* const GetWindow();

		template<typename T>
		static T* GetRendererAPI()
		{
			if (!std::is_base_of<RendererAPI, T>())
			{
				return nullptr;
			}

			return static_cast<T*>(GetRendererAPI());
		}

		template<typename T>
		static T* GetWindow()
		{
			if (!std::is_base_of<Window, T>())
			{
				return nullptr;
			}

			return static_cast<T*>(GetWindow());
		}

		static void SetRunning(const bool running);
		static const bool IsRunning();

		static const float GetDeltaTime();

	private:
		static RendererAPI* s_pRendererAPI;
		static Window* s_pWindow;
		static float s_DeltaTime;
		static bool s_Running;
};