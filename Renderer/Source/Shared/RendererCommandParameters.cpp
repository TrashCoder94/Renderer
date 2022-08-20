#include "Shared/RendererCommandParameters.h"
#include "Shared/RendererAPI.h"
#include "DirectX12/DirectX12.h"
#include "DirectX12/DirectX12CommandParameters.h"

RendererCommandParameters::RendererCommandParameters() : 
	m_WindowWidth(800),
	m_WindowHeight(600),
	m_Fullscreen(false)
{}

RendererCommandParameters::~RendererCommandParameters()
{}

const uint32_t RendererCommandParameters::GetWindowWidth()
{
	return m_WindowWidth;
}

void RendererCommandParameters::SetWindowWidth(const uint32_t width)
{
	m_WindowWidth = width;
}

const uint32_t RendererCommandParameters::GetWindowHeight()
{
	return m_WindowHeight;
}

void RendererCommandParameters::SetWindowHeight(const uint32_t height)
{
	m_WindowHeight = height;
}

const bool RendererCommandParameters::IsFullscreen()
{
	return m_Fullscreen;
}

void RendererCommandParameters::SetFullscreen(const bool fullscreen)
{
	m_Fullscreen = fullscreen;
}

RendererCommandParameters* RendererCommandParameters::Create()
{
	switch (RendererAPI::GetAPI())
	{
		case RendererAPI::API::DirectX12:
		{
			return new DirectX12CommandParameters();
		}
		case RendererAPI::API::Vulkan:
		{
			break;
		}
		default:
		{
			break;
		}
	}

	return nullptr;
}
