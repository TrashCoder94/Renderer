#pragma once

class RendererCommandParameters;

class Window
{
protected:
	explicit Window(const std::string& name, const uint32_t width, const uint32_t height);

public:
	virtual ~Window() = default;

	virtual void Initialize(RendererCommandParameters* pCommandParameters) = 0;
	virtual void Deinitialize() = 0;

	virtual void Show() = 0;
	virtual void Hide() = 0;

	virtual void SetVSync(bool enabled);
	virtual bool IsVSync() const;

	virtual void SetFullscreen(bool enabled);
	virtual bool IsFullscreen() const;

	inline void SetWidth(const uint32_t width)		{ m_Width = width; }
	inline void SetHeight(const uint32_t height)	{ m_Height = height; }
	
	inline const std::string& GetName()				{ return m_Name; }
	inline const uint32_t GetWidth()				{ return m_Width; }
	inline const uint32_t GetHeight()				{ return m_Height; }

	static Window* Create(const std::string& name, const uint32_t width, const uint32_t height);

protected:
	std::string m_Name;
	uint32_t m_Width;
	uint32_t m_Height;
	bool m_Vsync;
	bool m_Fullscreen;
};