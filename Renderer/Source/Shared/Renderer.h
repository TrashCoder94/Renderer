#pragma once

class RendererCommandParameters;
class RendererAPI;
class Window;

class Renderer
{
	public:
		static void Initialize(std::shared_ptr<RendererCommandParameters> pCommandParameters);
		static void Update();
		static void Deinitialize();

		static std::shared_ptr<RendererAPI>& GetRendererAPI();
		static std::shared_ptr<Window>& GetWindow();

		template<typename T>
		static std::shared_ptr<T> GetRendererAPI()
		{
			if (!std::is_base_of<RendererAPI, T>())
			{
				return nullptr;
			}

			return std::static_pointer_cast<T>(GetRendererAPI());
		}

		template<typename T>
		static std::shared_ptr<T> GetWindow()
		{
			if (!std::is_base_of<Window, T>())
			{
				return nullptr;
			}

			return std::static_pointer_cast<T>(GetWindow());
		}

		static void SetRunning(const bool running);
		static const bool IsRunning();

		static const float GetDeltaTime();

		static void SetAutoQuitTime(const float autoQuitTime);

	private:
		static std::shared_ptr<RendererAPI> s_pRendererAPI;
		static std::shared_ptr<Window> s_pWindow;
		static float s_DeltaTime;
		static float s_AutoQuitTime;
		static bool s_Running;
};