#include "RendererAPI.h"
#include "OpenGL/OpenGLRendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

RendererAPI* RendererAPI::Create()
{
	switch (s_API)
	{
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLRendererAPI();
		}
		default:
		{
			return nullptr;
		}
	}

	return nullptr;
}