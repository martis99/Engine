#include "pch.h"
#include "api/gfx/arenderer.h"
#include "gl_atypes.h"
#include "gl/gl_renderer.h"

ARenderer* arenderer_create(AContext* context) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));

	gl_cull_face_back();
	gl_front_face_cw();

	gl_blend_func(gl_afactor(A_SRC_ALPHA), gl_afactor(A_ONE_MINUS_SRC_ALPHA));

	gl_unpack_alignment();
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_depth_stencil_set(ARenderer* renderer, bool depth_enabled, bool stencil_enabled) {
	if (depth_enabled == 0) {
		gl_depth_test_disable();
	} else {
		gl_depth_test_enable();
	}
}

void arenderer_rasterizer_set(ARenderer* renderer, bool wireframe, bool cull_back) {
	if (wireframe == 0) {
		gl_polygon_mode_fill();
	} else {
		gl_polygon_mode_line();
	}

	if (cull_back == 0) {
		gl_cull_face_disable();
	} else {
		gl_cull_face_enable();
	}
}

void arenderer_blend_set(ARenderer* renderer, bool enabled) {
	if (enabled == 0) {
		gl_blend_disable();
	} else {
		gl_blend_enable();
	}
}