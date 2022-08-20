#pragma once

// In order to define a function called CreateWindow, the Windows macro needs to
// be undefined.
#if defined(CreateWindow)
#undef CreateWindow
#endif

#include "Shared/Window.h"

class DirectX12Window : public Window
{
public:
	explicit DirectX12Window(const std::string& name, const uint32_t width, const uint32_t height);
	~DirectX12Window();

	virtual void Initialize(std::shared_ptr<RendererCommandParameters> pCommandParameters) override;
	virtual void Deinitialize() override;
	
	virtual void Show() override;
	virtual void Hide() override;

	virtual void SetFullscreen(bool enabled) override;

	static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void RegisterWindowClass(const HINSTANCE& hInst, const wchar_t* windowClassName);
	void CreateWindow(const wchar_t* windowClassName, const HINSTANCE& hInst,
		const wchar_t* windowTitle, uint32_t width, uint32_t height, HWND& outHWND);

	void SetTearingSupported(const bool tearingSupported);
	bool IsTearingSupported() const;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Window handle.
	HWND m_HWND;

	// Window rectangle (used to toggle fullscreen state).
	RECT m_WindowRect;

	bool m_TearingSupported;

	friend class DirectX12RendererAPI;
};