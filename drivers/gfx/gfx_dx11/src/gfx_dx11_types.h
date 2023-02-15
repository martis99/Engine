#pragma once

#include "dx11/dx11_types.h"

#include <Windows.h>

struct AContext {
	HWND window;
	ID3D11Device *device;
	IDXGISwapChain *swap_chain;
	ID3D11DeviceContext *context;
	DX11Error error;
};
