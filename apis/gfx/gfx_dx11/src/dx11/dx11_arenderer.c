#include "api/gfx/arenderer.h"
#include "api/gfx/aframebuffer.h"
#include "dx11_atypes.h"
#include "dx11/dx11_renderer.h"

ARenderer* arenderer_create(AContext* context, LogCallbacks* log) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));
	renderer->device = context->device;
	renderer->context = context->context;
	renderer->acontext = context;
	renderer->error = &context->error;
	renderer->log = log;

	D3D11_VIEWPORT vp;
	vp.Width = 1600;
	vp.Height = 900;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	renderer->context->lpVtbl->RSSetViewports(renderer->context, 1, &vp);

	renderer->depth_stencil = dx11_depth_stencil_create(&context->error, renderer->device, FALSE, FALSE);
	if (renderer->depth_stencil == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil");
		return NULL;
	}

	renderer->depth_stencil_depth = dx11_depth_stencil_create(&context->error, renderer->device, TRUE, FALSE);
	if (renderer->depth_stencil_depth == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil depth");
		return NULL;
	}

	renderer->depth_stencil_stencil = dx11_depth_stencil_create(&context->error, renderer->device, FALSE, TRUE);
	if (renderer->depth_stencil_stencil == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil stencil");
		return NULL;
	}

	renderer->depth_stencil_depth_stencil = dx11_depth_stencil_create(&context->error, renderer->device, TRUE, TRUE);
	if (renderer->depth_stencil_depth_stencil == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil depth stencil");
		return NULL;
	}

	renderer->rasterizer_solid_none = dx11_resterizer_create(&context->error, renderer->device, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	if (renderer->rasterizer_solid_none == NULL) {
		log_msg(renderer->log, "Failed to create rasterizer solid none");
		return NULL;
	}

	renderer->rasterizer_solid_back = dx11_resterizer_create(&context->error, renderer->device, D3D11_FILL_SOLID, D3D11_CULL_BACK);
	if (renderer->rasterizer_solid_back == NULL) {
		log_msg(renderer->log, "Failed to create rasterizer solid back");
		return NULL;
	}

	renderer->rasterizer_wireframe_none = dx11_resterizer_create(&context->error, renderer->device, D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
	if (renderer->rasterizer_wireframe_none == NULL) {
		log_msg(renderer->log, "Failed to create rasterizer wireframe none");
		return NULL;
	}

	renderer->rasterizer_wireframe_back = dx11_resterizer_create(&context->error, renderer->device, D3D11_FILL_WIREFRAME, D3D11_CULL_BACK);
	if (renderer->rasterizer_wireframe_back == NULL) {
		log_msg(renderer->log, "Failed to create rasterizer wireframe back");
		return NULL;
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

void arenderer_delete(ARenderer* renderer) {
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
	if (renderer->rasterizer_solid_none != NULL) {
		dx11_rasterizer_delete(renderer->rasterizer_solid_none);
		renderer->rasterizer_solid_none = NULL;
	}
	if (renderer->rasterizer_solid_back != NULL) {
		dx11_rasterizer_delete(renderer->rasterizer_solid_back);
		renderer->rasterizer_solid_back = NULL;
	}
	if (renderer->rasterizer_wireframe_none != NULL) {
		dx11_rasterizer_delete(renderer->rasterizer_wireframe_none);
		renderer->rasterizer_wireframe_none = NULL;
	}
	if (renderer->rasterizer_wireframe_back != NULL) {
		dx11_rasterizer_delete(renderer->rasterizer_wireframe_back);
		renderer->rasterizer_wireframe_back = NULL;
	}
	if (renderer->rasterizer_wireframe_back != NULL) {
		dx11_rasterizer_delete(renderer->rasterizer_wireframe_back);
		renderer->rasterizer_wireframe_back = NULL;
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

void arenderer_depth_stencil_set(ARenderer* renderer, bool depth_enabled, bool stencil_enabled) {
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

void arenderer_rasterizer_set(ARenderer* renderer, bool wireframe, bool cull_back) {
	if (wireframe == 0) {
		if (cull_back == 0) {
			dx11_rasterizer_bind(renderer->rasterizer_solid_none, renderer->context);
		} else {
			dx11_rasterizer_bind(renderer->rasterizer_solid_back, renderer->context);
		}
	} else {
		if (cull_back == 0) {
			dx11_rasterizer_bind(renderer->rasterizer_wireframe_none, renderer->context);
		} else {
			dx11_rasterizer_bind(renderer->rasterizer_wireframe_back, renderer->context);
		}
	}
}

void arenderer_blend_set(ARenderer* renderer, bool enabled) {
	if (enabled == 0) {
		dx11_blend_bind(renderer->blend_disabled, renderer->context);
	} else {
		dx11_blend_bind(renderer->blend_enabled, renderer->context);
	}
}