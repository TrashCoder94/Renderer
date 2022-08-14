#pragma once

#include "Shared/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	OpenGLRendererAPI();
	~OpenGLRendererAPI();

	virtual void Initialize() override;
	virtual void Deinitialize() override;

	virtual const float GetTime() override;
};