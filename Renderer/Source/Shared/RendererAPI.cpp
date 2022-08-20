#include "rpch.h"
#include "RendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX12;

std::shared_ptr<RendererAPI> RendererAPI::Create(const RendererAPI::API api)
{
	s_API = api;

	switch (s_API)
	{
		case RendererAPI::API::DirectX12:
		{
			return std::make_shared<DirectX12RendererAPI>();
		}
		default:
		{
			return nullptr;
		}
	}

	return nullptr;
}