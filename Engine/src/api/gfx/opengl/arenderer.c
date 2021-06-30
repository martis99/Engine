#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/arenderer.h"

#include "gl/gl_renderer.h"

struct ARenderer {
	byte id;
};

ARenderer* arenderer_create() {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));

	gl_blend_set_enabled(1);
	gl_cull_face_set_enabled(1);
	gl_depth_test_set_enabled(1);
	gl_debug_output_set_enabled(1);

	gl_cull_face_back();
	gl_front_face_cw();
	gl_polygon_mode_fill();

	gl_blend_func(S_SRC_ALPHA, D_ONE_MINUS_SRC_ALPHA);
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_clear(ARenderer* renderer, float red, float green, float blue, float alpha) {
	gl_clear(red, green, blue, alpha);
}

void arenderer_clear_depth(ARenderer* renderer) {
	gl_clear_depth();
}

void arenderer_cull_face_set_enabled(ARenderer* renderer, bool enabled) {
	gl_cull_face_set_enabled(enabled);
}

void arenderer_polygon_mode_fill(ARenderer* renderer) {
	gl_polygon_mode_fill();
}

void arenderer_polygon_mode_line(ARenderer* renderer) {
	gl_polygon_mode_line();
}
#endif