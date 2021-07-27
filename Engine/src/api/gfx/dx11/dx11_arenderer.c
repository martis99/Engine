#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/arenderer.h"

#include <d3d11.h>

struct AContext {
	HWND window;
	ID3D11Device* device;
	IDXGISwapChain* swap_chain;
	ID3D11DeviceContext* context;
};

struct ARenderer {
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* target;
};

ARenderer* arenderer_create(AContext* context) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));
	renderer->context = context->context;

	ID3D11Resource* back_buffer = NULL;
	context->swap_chain->lpVtbl->GetBuffer(context->swap_chain, 0, &IID_ID3D11Resource, (void**)(&back_buffer));
	context->device->lpVtbl->CreateRenderTargetView(context->device, back_buffer, NULL, &renderer->target);
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
	FLOAT color[4] = { 1, 0, 0, 1 };
	renderer->context->lpVtbl->ClearRenderTargetView(renderer->context, renderer->target, color);
}

void arenderer_clear_buffer_depth(ARenderer* renderer) {

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