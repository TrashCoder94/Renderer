#include "DirectX12CommandParameters.h"
#include "DirectX12/DirectX12.h"

DirectX12CommandParameters::DirectX12CommandParameters() : RendererCommandParameters(),
	m_UseWarp(false)
{}

DirectX12CommandParameters::~DirectX12CommandParameters()
{}

const bool DirectX12CommandParameters::GetUseWarp()
{
	return m_UseWarp;
}

void DirectX12CommandParameters::SetUseWarp(const bool useWarp)
{
	m_UseWarp = useWarp;
}