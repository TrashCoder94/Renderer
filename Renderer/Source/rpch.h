#pragma once	

// Common
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cinttypes>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Data Structures
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Platform Specific
#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
	#include <shellapi.h> // For CommandLineToArgvW
	#include <cassert>
	#include <cstdint>
#endif

// Project Specific
#include <Shared/RendererAPI.h>
#include <Shared/RendererCommandParameters.h>
#include <Shared/Renderer.h>
#include <Shared/Window.h>

#ifdef PLATFORM_WINDOWS
	#include <DirectX12/DirectX12.h>
	#include <DirectX12/DirectX12RendererAPI.h>
	#include <DirectX12/DirectX12CommandParameters.h>
	#include <DirectX12/DirectX12Window.h>
#endif