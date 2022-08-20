#include "rpch.h"
#include "Window.h"

Window::Window(const std::string& name, const uint32_t width, const uint32_t height) :
	m_Name(name),
	m_Width(width),
	m_Height(height),
	m_Vsync(true),
	m_Fullscreen(false)
{}

Window* Window::Create(const std::string& name, const uint32_t width, const uint32_t height)
{
	switch (RendererAPI::GetAPI())
	{
		case RendererAPI::API::DirectX12:
		{
			return new DirectX12Window(name, width, height);
		}
		default:
		{
			return nullptr;
		}
	}

	return nullptr;
}

void Window::SetVSync(bool enabled)
{
	m_Vsync = enabled;
}

bool Window::IsVSync() const
{
	return m_Vsync;
}

void Window::SetFullscreen(bool enabled)
{
	m_Fullscreen = enabled;
}

bool Window::IsFullscreen() const
{
	return m_Fullscreen;
}
