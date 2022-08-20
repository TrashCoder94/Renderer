#pragma once

#include "Shared/RendererCommandParameters.h"

class DirectX12CommandParameters : public RendererCommandParameters
{
public:
	DirectX12CommandParameters();
	~DirectX12CommandParameters();

	const bool GetUseWarp();
	void SetUseWarp(const bool useWarp);

protected:
	bool m_UseWarp;
};