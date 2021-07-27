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

	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	if (renderer->target != NULL) {
		renderer->target->lpVtbl->Release(renderer->target);
	}
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_clear_buffers(ARenderer* renderer) {

}

void arenderer_clear_buffer_color(ARenderer* renderer) {
	FLOAT color[4] = {0.1, 0.1, 0.1, 1.0 };
	renderer->context->lpVtbl->ClearRenderTargetView(renderer->context, renderer->target, color);
}

void arenderer_clear_buffer_depth(ARenderer* renderer) {
	renderer->context->lpVtbl->OMSetRenderTargets(renderer->context, 1, &renderer->target, NULL);

	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width = 1600;
	vp.Height = 900;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	renderer->context->lpVtbl->RSSetViewports(renderer->context, 1, &vp);
}

void arenderer_clear_color(ARenderer* renderer, float red, float green, float blue, float alpha) {
	FLOAT color[4] = { red, green, blue, alpha };
	renderer->context->lpVtbl->ClearRenderTargetView(renderer->context, renderer->target, color);
}

void arenderer_depth_test_set_enabled(ARenderer* renderer, bool enabled) {

}

void arenderer_cull_face_set_enabled(ARenderer* renderer, bool enabled) {

}

void arenderer_polygon_mode_fill(ARenderer* renderer) {

}

void arenderer_polygon_mode_line(ARenderer* renderer) {

}
#endif