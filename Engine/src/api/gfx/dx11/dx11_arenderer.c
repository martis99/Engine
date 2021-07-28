#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/arenderer.h"
#include "dx11_astructs.h"

ARenderer* arenderer_create(AContext* context) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));
	renderer->device = context->device;
	renderer->context = context->context;

	ID3D11Resource* back_buffer = NULL;
	HRESULT hr;
	hr = context->swap_chain->lpVtbl->GetBuffer(context->swap_chain, 0, &IID_ID3D11Resource, (void**)(&back_buffer));
	if (FAILED(hr)) {
		log_error("Failed to get back buffer");
		return NULL;
	}
	hr = context->device->lpVtbl->CreateRenderTargetView(context->device, back_buffer, NULL, &renderer->target);
	if (FAILED(hr)) {
		log_error("Failed to create render target view");
		return NULL;
	}
	back_buffer->lpVtbl->Release(back_buffer);

	D3D11_DEPTH_STENCIL_DESC dsd = { 0 };
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	hr = renderer->device->lpVtbl->CreateDepthStencilState(renderer->device, &dsd, &renderer->dsState);
	if (FAILED(hr)) {
		log_error("Failed to create depth stencil state");
		return NULL;
	}

	renderer->context->lpVtbl->OMSetDepthStencilState(renderer->context, renderer->dsState, 1);

	D3D11_TEXTURE2D_DESC dtd = { 0 };
	dtd.Width = 1600;
	dtd.Height = 900;
	dtd.MipLevels = 1;
	dtd.ArraySize = 1;
	dtd.Format = DXGI_FORMAT_D32_FLOAT;
	dtd.SampleDesc.Count = 1;
	dtd.SampleDesc.Quality = 0;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = renderer->device->lpVtbl->CreateTexture2D(renderer->device, &dtd, NULL, &renderer->depthStencil);
	if (FAILED(hr)) {
		log_error("Failed to create texture");
		return NULL;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = { 0 };
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = renderer->device->lpVtbl->CreateDepthStencilView(renderer->device, (ID3D11Resource*)renderer->depthStencil, &dsvd, &renderer->dsv);
	if (FAILED(hr)) {
		log_error("Failed to create depth stencil view");
		return NULL;
	}

	renderer->context->lpVtbl->OMSetRenderTargets(renderer->context, 1, &renderer->target, renderer->dsv);

	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width = 1600;
	vp.Height = 900;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	renderer->context->lpVtbl->RSSetViewports(renderer->context, 1, &vp);

	D3D11_RASTERIZER_DESC sd = { 0 };
	sd.FillMode = D3D11_FILL_SOLID;
	sd.CullMode = D3D11_CULL_NONE;
	sd.FrontCounterClockwise = 0;

	D3D11_RASTERIZER_DESC scd = { 0 };
	scd.FillMode = D3D11_FILL_SOLID;
	scd.CullMode = D3D11_CULL_BACK;
	scd.FrontCounterClockwise = 0;

	D3D11_RASTERIZER_DESC wd = { 0 };
	wd.FillMode = D3D11_FILL_WIREFRAME;
	wd.CullMode = D3D11_CULL_NONE;
	wd.FrontCounterClockwise = 0;

	D3D11_RASTERIZER_DESC wcd = { 0 };
	wcd.FillMode = D3D11_FILL_WIREFRAME;
	wcd.CullMode = D3D11_CULL_BACK;
	wcd.FrontCounterClockwise = 0;

	hr = renderer->device->lpVtbl->CreateRasterizerState(renderer->device, &sd, &renderer->raster_s);
	if (FAILED(hr)) {
		log_error("Failed to create resterizer state s");
		return NULL;
	}
	hr = renderer->device->lpVtbl->CreateRasterizerState(renderer->device, &scd, &renderer->raster_sc);
	if (FAILED(hr)) {
		log_error("Failed to create resterizer state sc");
		return NULL;
	}
	hr = renderer->device->lpVtbl->CreateRasterizerState(renderer->device, &wd, &renderer->raster_w);
	if (FAILED(hr)) {
		log_error("Failed to create resterizer state w");
		return NULL;
	}
	hr = renderer->device->lpVtbl->CreateRasterizerState(renderer->device, &wcd, &renderer->raster_wc);
	if (FAILED(hr)) {
		log_error("Failed to create resterizer state wc");
		return NULL;
	}

	renderer->context->lpVtbl->RSSetState(renderer->context, renderer->raster_sc);
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	if (renderer->target != NULL) {
		renderer->target->lpVtbl->Release(renderer->target);
	}
	if (renderer->dsState != NULL) {
		renderer->dsState->lpVtbl->Release(renderer->dsState);
	}
	if (renderer->depthStencil != NULL) {
		renderer->depthStencil->lpVtbl->Release(renderer->depthStencil);
	}
	if (renderer->dsv != NULL) {
		renderer->dsv->lpVtbl->Release(renderer->dsv);
	}
	if (renderer->raster_s != NULL) {
		renderer->raster_s->lpVtbl->Release(renderer->raster_s);
	}
	if (renderer->raster_sc != NULL) {
		renderer->raster_sc->lpVtbl->Release(renderer->raster_sc);
	}
	if (renderer->raster_w != NULL) {
		renderer->raster_w->lpVtbl->Release(renderer->raster_w);
	}
	if (renderer->raster_wc != NULL) {
		renderer->raster_wc->lpVtbl->Release(renderer->raster_wc);
	}
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_clear_buffers(ARenderer* renderer) {

}

void arenderer_clear_buffer_color(ARenderer* renderer) {
}

void arenderer_clear_buffer_depth(ARenderer* renderer) {

}

void arenderer_clear_color(ARenderer* renderer, float red, float green, float blue, float alpha) {
	FLOAT color[4] = { red, green, blue, alpha };
	renderer->context->lpVtbl->ClearRenderTargetView(renderer->context, renderer->target, color);
	renderer->context->lpVtbl->ClearDepthStencilView(renderer->context, renderer->dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void arenderer_depth_test_set_enabled(ARenderer* renderer, bool enabled) {

}

void arenderer_cull_face_set_enabled(ARenderer* renderer, bool enabled) {
	if (enabled == 1) {
		renderer->context->lpVtbl->RSSetState(renderer->context, renderer->raster_sc);
	} else {
		renderer->context->lpVtbl->RSSetState(renderer->context, renderer->raster_s);
	}
}

void arenderer_polygon_mode_fill(ARenderer* renderer) {
	renderer->context->lpVtbl->RSSetState(renderer->context, renderer->raster_sc);
}

void arenderer_polygon_mode_line(ARenderer* renderer) {
	renderer->context->lpVtbl->RSSetState(renderer->context, renderer->raster_w);
}
#endif