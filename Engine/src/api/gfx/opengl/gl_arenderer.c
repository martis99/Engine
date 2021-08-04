#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/arenderer.h"
#include "gl_atypes.h"
#include "gl/gl_renderer.h"

ARenderer* arenderer_create(AContext* context) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));

	gl_blend_set_enabled(1);
	gl_cull_face_set_enabled(1);
	gl_depth_test_set_enabled(1);
	gl_debug_output_set_enabled(1);

	gl_cull_face_back();
	gl_front_face_cw();
	gl_polygon_mode_fill();

	gl_blend_func(gl_afactor(A_SRC_ALPHA), gl_afactor(A_ONE_MINUS_SRC_ALPHA));

	gl_unpack_alignment();
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_clear_buffers(ARenderer* renderer) {
	gl_clear_buffers();
}

void arenderer_clear_buffer_color(ARenderer* renderer) {
	gl_clear_buffer_color();
}

void arenderer_clear_buffer_depth(ARenderer* renderer) {
	gl_clear_buffer_depth();
}

void arenderer_clear_color(ARenderer* renderer, float red, float green, float blue, float alpha) {
	gl_clear_color(red, green, blue, alpha);
}

void arenderer_depth_test_set_enabled(ARenderer* renderer, bool enabled) {
	gl_depth_test_set_enabled(enabled);
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