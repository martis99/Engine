#include "api/gfx/gfx_api_framebuffer.h"
#include "api/gfx/gfx_api_renderer.h"
#include "dx11/dx11_renderer.h"

#include "gfx_dx11_types.h"

#include <math/mat4.h>

ARenderer *arenderer_create(AContext *context, LogCallbacks *log, int lhc)
{
	ARenderer *renderer = m_malloc(sizeof(ARenderer));
	renderer->device    = context->device;
	renderer->context   = context->context;
	renderer->acontext  = context;
	renderer->error	    = &context->error;
	renderer->log	    = log;
	renderer->lhc	    = lhc;

	D3D11_VIEWPORT vp = {
		.Width	  = 1600,
		.Height	  = 900,
		.MinDepth = 0,
		.MaxDepth = 1,
		.TopLeftX = 0,
		.TopLeftY = 0,
	};

	renderer->context->lpVtbl->RSSetViewports(renderer->context, 1, &vp);

	D3D11_COMPARISON_FUNC depth = dx11_adepth_func(lhc == 1 ? A_DEPTH_LEQUAL : A_DEPTH_GEQUAL);
	renderer->depth_stencil	    = dx11_depth_stencil_create(&context->error, renderer->device, FALSE, FALSE, depth);
	if (renderer->depth_stencil == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil");
		return NULL;
	}

	renderer->depth_stencil_depth = dx11_depth_stencil_create(&context->error, renderer->device, TRUE, FALSE, depth);
	if (renderer->depth_stencil_depth == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil depth");
		return NULL;
	}

	renderer->depth_stencil_stencil = dx11_depth_stencil_create(&context->error, renderer->device, FALSE, TRUE, depth);
	if (renderer->depth_stencil_stencil == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil stencil");
		return NULL;
	}

	renderer->depth_stencil_depth_stencil = dx11_depth_stencil_create(&context->error, renderer->device, TRUE, TRUE, depth);
	if (renderer->depth_stencil_depth_stencil == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil depth stencil");
		return NULL;
	}

	for (int i = 0; i < 8; i++) {
		BOOL ccw	     = i / 4 % 2;
		D3D11_FILL_MODE fill = i / 2 % 2 == 0 ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
		D3D11_CULL_MODE cull = i % 2 == 0 ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		renderer->raster_states.states[i] = dx11_resterizer_create(&context->error, renderer->device, fill, cull, ccw);
		if (renderer->raster_states.cw_solid == NULL) {
			log_msg(renderer->log, "Failed to create rasterizer");
			return NULL;
		}
	}

	renderer->blend_enabled = dx11_blend_create(&context->error, renderer->device, TRUE);
	if (renderer->blend_enabled == NULL) {
		log_msg(renderer->log, "Failed to create blend enabled");
		return NULL;
	}

	renderer->blend_disabled = dx11_blend_create(&context->error, renderer->device, FALSE);
	if (renderer->blend_disabled == NULL) {
		log_msg(renderer->log, "Failed to create blend disabled");
		return NULL;
	}

	return renderer;
}

void arenderer_delete(ARenderer *renderer)
{
	if (renderer->depth_stencil != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil);
		renderer->depth_stencil = NULL;
	}
	if (renderer->depth_stencil_depth != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil_depth);
		renderer->depth_stencil_depth = NULL;
	}
	if (renderer->depth_stencil_stencil != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil_stencil);
		renderer->depth_stencil_stencil = NULL;
	}
	if (renderer->depth_stencil_depth_stencil != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil_depth_stencil);
		renderer->depth_stencil_depth_stencil = NULL;
	}
	for (int i = 0; i < 8; i++) {
		if (renderer->raster_states.states[i] != NULL) {
			dx11_rasterizer_delete(renderer->raster_states.states[i]);
			renderer->raster_states.states[i] = NULL;
		}
	}
	if (renderer->blend_enabled != NULL) {
		dx11_blend_delete(renderer->blend_enabled);
		renderer->blend_enabled = NULL;
	}
	if (renderer->blend_disabled != NULL) {
		dx11_blend_delete(renderer->blend_disabled);
		renderer->blend_disabled = NULL;
	}
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_depth_stencil_set(ARenderer *renderer, bool depth_enabled, bool stencil_enabled)
{
	if (depth_enabled == 0) {
		if (stencil_enabled == 0) {
			dx11_depth_stencil_bind(renderer->depth_stencil, renderer->context);
		} else {
			dx11_depth_stencil_bind(renderer->depth_stencil_stencil, renderer->context);
		}
	} else {
		if (stencil_enabled == 0) {
			dx11_depth_stencil_bind(renderer->depth_stencil_depth, renderer->context);
		} else {
			dx11_depth_stencil_bind(renderer->depth_stencil_depth_stencil, renderer->context);
		}
	}
}

void arenderer_rasterizer_set(ARenderer *renderer, bool wireframe, bool cull_back, bool ccw)
{
	renderer->cull_back = cull_back;
	dx11_rasterizer_bind(renderer->raster_states.states[ccw * 4 + wireframe * 2 + cull_back], renderer->context);
}

void arenderer_blend_set(ARenderer *renderer, bool enabled)
{
	if (enabled == 0) {
		dx11_blend_bind(renderer->blend_disabled, renderer->context);
	} else {
		dx11_blend_bind(renderer->blend_enabled, renderer->context);
	}
}

mat4 arenderer_perspective(ARenderer *renderer, float fovy, float aspect, float zNear, float zFar)
{
	return mat4_perspective0(fovy, aspect, zNear, zFar, renderer->lhc);
}

mat4 arenderer_ortho(ARenderer *renderer, float left, float right, float bottom, float top, float znear, float zfar)
{
	return mat4_ortho0(left, right, bottom, top, znear, zfar, renderer->lhc);
}

float arenderer_near(ARenderer *renderer)
{
	return 0;
}

float arenderer_far(ARenderer *renderer)
{
	return 1;
}
