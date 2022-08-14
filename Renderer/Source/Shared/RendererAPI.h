#pragma once

class RendererAPI
{
public:
	enum class API
	{
		OpenGL,
		DirectX11,
		DirectX12,
		Vulkan
	};

	virtual ~RendererAPI() = default;

	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;

	virtual const float GetTime() = 0;

	static inline API& GetAPI() { return s_API; }

	static RendererAPI* Create();

private:
	static API s_API;
};