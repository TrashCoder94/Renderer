#pragma once

#include <string>

class Window
{
protected:
	explicit Window(const std::string& name, const uint32_t width, const uint32_t height);

public:
	virtual ~Window() = default;

	virtual void Initialize() = 0;
	virtual bool Update(const float deltaTime) = 0;
	virtual void Deinitialize() = 0;

	static Window* Create(const std::string& name, const uint32_t width, const uint32_t height);

protected:
	std::string m_Name;
	uint32_t m_Width;
	uint32_t m_Height;
};