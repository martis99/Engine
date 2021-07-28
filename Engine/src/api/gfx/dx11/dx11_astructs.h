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
	ID3D11DepthStencilState* dsState;
	ID3D11Texture2D* depthStencil;
	ID3D11DepthStencilView* dsv;
	ID3D11RasterizerState* raster_s;
	ID3D11RasterizerState* raster_sc;
	ID3D11RasterizerState* raster_w;
	ID3D11RasterizerState* raster_wc;
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
	ID3D11InputLayout* il;

	UINT count;
	UINT vertex_size;
	APrimitive primitive;
};

struct ATexture {
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* ss;
};

struct AUniformBuffer {
	ID3D11Buffer* cb;
};
#endif