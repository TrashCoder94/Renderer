#include "rpch.h"
#include "RendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX12;

RendererAPI* RendererAPI::Create(const RendererAPI::API api)
{
	s_API = api;

	switch (s_API)
	{
		case RendererAPI::API::DirectX12:
		{
			return new DirectX12RendererAPI();
		}
		default:
		{
			return nullptr;
		}
	}

	return nullptr;
}