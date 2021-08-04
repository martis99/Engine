#pragma once
#ifdef GAPI_DX11
#include "api/atypes.h"

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
	ID3D11BlendState* blendState;
};

struct AShader {
	ID3DBlob* vs_blob;
	ID3D11VertexShader* vs;
	ID3DBlob* ps_blob;
	ID3D11PixelShader* ps;
};

struct AMesh {
	UINT vertex_size;
	ID3D11Buffer* vertices;
	UINT vertices_count;

	UINT instance_size;
	ID3D11Buffer* instances;
	UINT instances_count;

	UINT index_size;
	ID3D11Buffer* indices;
	UINT indices_count;
	DXGI_FORMAT index_format;

	ID3D11InputLayout* layout;
	D3D11_PRIMITIVE_TOPOLOGY primitive;
};

struct ATexture {
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* ss;
};

struct AFramebuffer {
	uint id;
};

struct ABuffer {
	UINT* offsets;
	UINT* sizes;
	UINT size;
	UINT count;
	void* data;
};

struct AUniformBuffer {
	ID3D11Buffer* cb;
	ABuffer* buffer;
};

struct AMaterial {
	ID3D11Buffer* cb;
	ABuffer* buffer;
};

D3D11_PRIMITIVE_TOPOLOGY dx11_aprimitive(APrimitive primitive);
D3D11_TEXTURE_ADDRESS_MODE dx11_awrap(AWrap wrap);
D3D11_FILTER dx11_afilter(AFilter filter);
DXGI_FORMAT dx11_atype_format(AType type);

#endif