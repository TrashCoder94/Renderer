#include "Window.h"
#include "Shared/RendererAPI.h"
#include "OpenGL/OpenGLWindow.h"

Window::Window(const std::string& name, const uint32_t width, const uint32_t height) :
	m_Name(name),
	m_Width(width),
	m_Height(height)
{}

Window* Window::Create(const std::string& name, const uint32_t width, const uint32_t height)
{
	switch (RendererAPI::GetAPI())
	{
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLWindow(name, width, height);
		}
		default:
		{
			return nullptr;
		}
	}

	return nullptr;
}