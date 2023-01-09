#pragma once
#include "dx11/dx11_types.h"

struct AContext {
	HWND window;
	ID3D11Device* device;
	IDXGISwapChain* swap_chain;
	ID3D11DeviceContext* context;
	DX11Error error;
};

typedef union RasterizerStates {
	ID3D11RasterizerState* states[8];
	struct {
		ID3D11RasterizerState* cw_solid, * cw_solid_cull, * cw_wire, * cw_wire_cull;
		ID3D11RasterizerState* ccw_solid, * ccw_solid_cull, * ccw_wire, * ccw_wire_cull;
	};

} RasterizerStates;

struct ARenderer {
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	AContext* acontext;
	ID3D11DepthStencilState* depth_stencil;
	ID3D11DepthStencilState* depth_stencil_depth;
	ID3D11DepthStencilState* depth_stencil_stencil;
	ID3D11DepthStencilState* depth_stencil_depth_stencil;
	ID3D11BlendState* blend_enabled;
	ID3D11BlendState* blend_disabled;
	RasterizerStates raster_states;
	DX11Error* error;
	LogCallbacks* log;
	int cull_back;
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
D3D11_COMPARISON_FUNC dx11_adepth_func(ADepthFunc func);
DXGI_FORMAT dx11_atype_format(AType type);