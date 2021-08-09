#pragma once
#ifdef GAPI_DX11
#include "api/atypes.h"

#include "dx11/dx11.h"

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
	AContext* acontext;
	ID3D11DepthStencilState* depth_stencil;
	ID3D11DepthStencilState* depth_stencil_depth;
	ID3D11DepthStencilState* depth_stencil_stencil;
	ID3D11DepthStencilState* depth_stencil_depth_stencil;
	ID3D11RasterizerState* rasterizer_solid_none;
	ID3D11RasterizerState* rasterizer_solid_back;
	ID3D11RasterizerState* rasterizer_wireframe_none;
	ID3D11RasterizerState* rasterizer_wireframe_back;
	ID3D11BlendState* blend_enabled;
	ID3D11BlendState* blend_disabled;
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

typedef struct DX11Attachment {
	AType type;
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* ss;
	ID3D11Texture2D* st;
	uint pixel_size;
} DX11Attachment;

struct AFramebuffer {
	ID3D11RenderTargetView* rtv;

	uint attachments_count;
	DX11Attachment** attachments;

	ID3D11Texture2D* dst;
	ID3D11DepthStencilView* dsv;

	AShader* shader;
	AMesh* mesh;
};

struct AUniformBuffer {
	ID3D11Buffer* buffer;
	UINT slot;
};

D3D11_PRIMITIVE_TOPOLOGY dx11_aprimitive(APrimitive primitive);
D3D11_TEXTURE_ADDRESS_MODE dx11_awrap(AWrap wrap);
D3D11_FILTER dx11_afilter(AFilter filter);
DXGI_FORMAT dx11_atype_format(AType type);

#endif