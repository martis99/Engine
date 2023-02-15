#include "dx11_buffer.h"
#include "dx11_error.h"

ID3D11BlendState *dx11_blend_create(DX11Error *error, ID3D11Device *device, BOOL enabled)
{
	ID3D11BlendState *blend = NULL;

	D3D11_BLEND_DESC desc		     = { 0 };
	D3D11_RENDER_TARGET_BLEND_DESC *rtbd = &desc.RenderTarget[0];
	rtbd->BlendEnable		     = enabled;
	rtbd->SrcBlend			     = D3D11_BLEND_SRC_ALPHA;
	rtbd->DestBlend			     = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd->BlendOp			     = D3D11_BLEND_OP_ADD;
	rtbd->SrcBlendAlpha		     = D3D11_BLEND_ZERO;
	rtbd->DestBlendAlpha		     = D3D11_BLEND_ZERO;
	rtbd->BlendOpAlpha		     = D3D11_BLEND_OP_ADD;
	rtbd->RenderTargetWriteMask	     = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (DX11_FAILED(error, "Failed to create blend state", device->lpVtbl->CreateBlendState(device, &desc, &blend))) {
		return NULL;
	}
	return blend;
}

void dx11_blend_bind(ID3D11BlendState *blend, ID3D11DeviceContext *context)
{
	context->lpVtbl->OMSetBlendState(context, blend, NULL, 0xFFFFFFFF);
}

void dx11_blend_delete(ID3D11BlendState *blend)
{
	blend->lpVtbl->Release(blend);
}

ID3D11RasterizerState *dx11_resterizer_create(DX11Error *error, ID3D11Device *device, D3D11_FILL_MODE fill_mode, D3D11_CULL_MODE cull_mode, BOOL ccw)
{
	ID3D11RasterizerState *rasterizer = NULL;

	D3D11_RASTERIZER_DESC desc = {
		.FillMode	       = fill_mode,
		.CullMode	       = cull_mode,
		.FrontCounterClockwise = ccw,
	};

	if (DX11_FAILED(error, "Failed to create resterizer state", device->lpVtbl->CreateRasterizerState(device, &desc, &rasterizer))) {
		return NULL;
	}
	return rasterizer;
}

void dx11_rasterizer_bind(ID3D11RasterizerState *rasterizer, ID3D11DeviceContext *context)
{
	context->lpVtbl->RSSetState(context, rasterizer);
}

void dx11_rasterizer_delete(ID3D11RasterizerState *rasterizer)
{
	rasterizer->lpVtbl->Release(rasterizer);
}

ID3D11DepthStencilState *dx11_depth_stencil_create(DX11Error *error, ID3D11Device *device, BOOL depth_enable, BOOL stencil_enable, D3D11_COMPARISON_FUNC depth_func)
{
	ID3D11DepthStencilState *depth_stencil = NULL;

	D3D11_DEPTH_STENCIL_DESC desc = {
		.DepthEnable	= depth_enable,
		.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		.DepthFunc	= depth_func,

		.StencilEnable	  = stencil_enable,
		.StencilReadMask  = 0xFF,
		.StencilWriteMask = 0xFF,

		.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP,
		.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR,
		.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP,
		.FrontFace.StencilFunc	      = D3D11_COMPARISON_ALWAYS,

		.BackFace.StencilFailOp	     = D3D11_STENCIL_OP_KEEP,
		.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR,
		.BackFace.StencilPassOp	     = D3D11_STENCIL_OP_KEEP,
		.BackFace.StencilFunc	     = D3D11_COMPARISON_ALWAYS,
	};

	if (DX11_FAILED(error, "Failed to create depth stencil state", device->lpVtbl->CreateDepthStencilState(device, &desc, &depth_stencil))) {
		return NULL;
	}
	return depth_stencil;
}

void dx11_depth_stencil_bind(ID3D11DepthStencilState *depth_stencil, ID3D11DeviceContext *context)
{
	context->lpVtbl->OMSetDepthStencilState(context, depth_stencil, 1);
}

void dx11_depth_stencil_delete(ID3D11DepthStencilState *depth_stencil)
{
	depth_stencil->lpVtbl->Release(depth_stencil);
}
