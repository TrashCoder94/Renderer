#include "RendererAPI.h"
#include "DirectX12/DirectX12.h"
#include "DirectX12/DirectX12RendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX12;

RendererAPI* RendererAPI::Create()
{
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