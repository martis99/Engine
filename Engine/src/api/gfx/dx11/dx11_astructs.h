#pragma once
#ifdef GAPI_DX11
#include "api/aenums.h"

#include <d3d11.h>

struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
	AWindowCallbacks callbacks;
	ACursor* cursor;
};

struct AContext {
	HWND window;
	ID3D11Device* device;
	IDXGISwapChain* swap_chain;
	ID3D11DeviceContext* context;
};

struct ARenderer {
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* target;
};

struct AShader {
	ID3DBlob* vs_blob;
	ID3D11VertexShader* vs;
	ID3DBlob* ps_blob;
	ID3D11PixelShader* ps;
};

struct AMesh {
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	APrimitive primitive;
};
#endif