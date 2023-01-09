#include "api/gfx/gfx_api_renderer.h"
#include "gl/gl_renderer.h"

#include "gfx_gl_types.h"

ARenderer* arenderer_create(AContext* context, LogCallbacks* log) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));
	renderer->error = &context->error;
	renderer->log = log;

	gl_cull_face_back(renderer->error);
	gl_front_face_cw(renderer->error);

	gl_blend_func(renderer->error, gl_afactor(A_SRC_ALPHA), gl_afactor(A_ONE_MINUS_SRC_ALPHA));
	gl_depth_func(renderer->error, gl_adepth_func(A_DEPTH_LEQUAL));

	gl_unpack_alignment(renderer->error);
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_depth_stencil_set(ARenderer* renderer, bool depth_enabled, bool stencil_enabled) {
	if (depth_enabled == 0) {
		gl_depth_test_disable(renderer->error);
	} else {
		gl_depth_test_enable(renderer->error);
	}
}

void arenderer_rasterizer_set(ARenderer* renderer, bool wireframe, bool cull_back) {
	if (wireframe == 0) {
		gl_polygon_mode_fill(renderer->error);
	} else {
		gl_polygon_mode_line(renderer->error);
	}

	if (cull_back == 0) {
		gl_cull_face_disable(renderer->error);
	} else {
		gl_cull_face_enable(renderer->error);
	}
}

void arenderer_blend_set(ARenderer* renderer, bool enabled) {
	if (enabled == 0) {
		gl_blend_disable(renderer->error);
	} else {
		gl_blend_enable(renderer->error);
	}
}