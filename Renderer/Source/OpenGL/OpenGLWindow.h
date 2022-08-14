#pragma once

#include "Shared/Window.h"

struct GLFWwindow;

class OpenGLWindow : public Window
{
public:
	OpenGLWindow(const std::string& name, const uint32_t width, const uint32_t height);
	~OpenGLWindow();

	virtual void Initialize() override;
	virtual bool Update(const float deltaTime) override;
	virtual void Deinitialize() override;

private:
	GLFWwindow* m_pGLFWWindow;
};