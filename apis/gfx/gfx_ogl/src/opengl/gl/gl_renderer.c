#include "gl_renderer.h"
#include "gl_error.h"

void gl_cull_face_enable() {
	GL_ASSERT(glEnable(GL_CULL_FACE));
}

void gl_cull_face_disable() {
	GL_ASSERT(glDisable(GL_CULL_FACE));
}

void gl_depth_test_enable() {
	GL_ASSERT(glEnable(GL_DEPTH_TEST));
}

void gl_depth_test_disable() {
	GL_ASSERT(glDisable(GL_DEPTH_TEST));
}

void gl_polygon_mode_fill() {
	GL_ASSERT(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

void gl_polygon_mode_line() {
	GL_ASSERT(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
}

void gl_cull_face_back() {
	GL_ASSERT(glCullFace(GL_BACK));
}

void gl_front_face_cw() {
	GL_ASSERT(glFrontFace(GL_CW));
}

void gl_unpack_alignment() {
	GL_ASSERT(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
}

void gl_blend_enable() {
	GL_ASSERT(glEnable(GL_BLEND));
}

void gl_blend_disable() {
	GL_ASSERT(glDisable(GL_BLEND));
}

void gl_blend_func(GLenum sfactor, GLenum dfactor) {
	GL_ASSERT(glBlendFunc(sfactor, dfactor));
}