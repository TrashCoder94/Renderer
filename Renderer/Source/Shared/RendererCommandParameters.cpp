#include "rpch.h"
#include "Shared/RendererCommandParameters.h"

RendererCommandParameters::RendererCommandParameters() : 
	m_API(RendererAPI::API::DirectX12),
	m_WindowWidth(800),
	m_WindowHeight(600),
	m_WindowName(),
	m_Fullscreen(false)
{}

RendererCommandParameters::~RendererCommandParameters()
{}

const RendererAPI::API RendererCommandParameters::GetAPI()
{
	return m_API;
}

void RendererCommandParameters::SetRendererAPI(const RendererAPI::API api)
{
	m_API = api;
}

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

const std::string& RendererCommandParameters::GetWindowName()
{
	return m_WindowName;
}

void RendererCommandParameters::SetWindowName(const std::string& name)
{
	m_WindowName = name;
}

const bool RendererCommandParameters::IsFullscreen()
{
	return m_Fullscreen;
}

void RendererCommandParameters::SetFullscreen(const bool fullscreen)
{
	m_Fullscreen = fullscreen;
}

std::shared_ptr<RendererCommandParameters> RendererCommandParameters::Create(const RendererAPI::API api)
{
	std::shared_ptr<RendererCommandParameters> pCommandParameters = nullptr;
	switch (api)
	{
		case RendererAPI::API::DirectX12:
		{
			pCommandParameters = std::make_shared<DirectX12CommandParameters>();
			pCommandParameters->SetRendererAPI(api);
			return pCommandParameters;
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

	return pCommandParameters;
}
