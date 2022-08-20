#pragma once

class RendererCommandParameters;

class RendererAPI
{
public:
	enum class API
	{
		DirectX12,
		Vulkan
	};

	virtual ~RendererAPI() = default;

	virtual void Initialize(RendererCommandParameters* pCommandParameters) = 0;
	virtual void Render() = 0;
	virtual bool Update(const float deltaTime) = 0;
	virtual void Deinitialize() = 0;

	const bool IsInitialized() { return m_IsInitialized; }

	static inline API& GetAPI() { return s_API; }

	static RendererAPI* Create(const RendererAPI::API api);

protected:
	bool m_IsInitialized;

private:
	static API s_API;
};