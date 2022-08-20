#pragma once

enum class RendererAPI::API;

class RendererCommandParameters
{
protected:
	RendererCommandParameters();

public:
	virtual ~RendererCommandParameters();

	const RendererAPI::API GetAPI();
	void SetRendererAPI(const RendererAPI::API api);

	const uint32_t GetWindowWidth();
	void SetWindowWidth(const uint32_t width);

	const uint32_t GetWindowHeight();
	void SetWindowHeight(const uint32_t height);

	const bool IsFullscreen();
	void SetFullscreen(const bool fullscreen);

	static RendererCommandParameters* Create(const RendererAPI::API api);

protected:
	RendererAPI::API m_API;
	uint32_t m_WindowWidth = 0;
	uint32_t m_WindowHeight = 0;
	bool m_Fullscreen = false;
};