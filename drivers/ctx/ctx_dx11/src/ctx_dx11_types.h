#pragma once

#include "dx11_ctx/dx11_ctx_types.h"

#include <Windows.h>

struct AContext {
	HWND window;
	ID3D11Device *device;
	IDXGISwapChain *swap_chain;
	ID3D11DeviceContext *context;
	DX11Error error;
};
