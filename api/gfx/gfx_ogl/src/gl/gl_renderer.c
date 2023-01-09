#include "gl_renderer.h"
#include "gl_error.h"

void gl_cull_face_enable(GLError* error) {
	GL_ASSERT(error, glEnable(GL_CULL_FACE));
}

void gl_cull_face_disable(GLError* error) {
	GL_ASSERT(error, glDisable(GL_CULL_FACE));
}

void gl_depth_test_enable(GLError* error) {
	GL_ASSERT(error, glEnable(GL_DEPTH_TEST));
}

void gl_depth_test_disable(GLError* error) {
	GL_ASSERT(error, glDisable(GL_DEPTH_TEST));
}

void gl_polygon_mode_fill(GLError* error) {
	GL_ASSERT(error, glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

void gl_polygon_mode_line(GLError* error) {
	GL_ASSERT(error, glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
}

void gl_cull_face_back(GLError* error) {
	GL_ASSERT(error, glCullFace(GL_BACK));
}

void gl_front_face_cw(GLError* error) {
	GL_ASSERT(error, glFrontFace(GL_CW));
}

void gl_unpack_alignment(GLError* error) {
	GL_ASSERT(error, glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
}

void gl_blend_enable(GLError* error) {
	GL_ASSERT(error, glEnable(GL_BLEND));
}

void gl_blend_disable(GLError* error) {
	GL_ASSERT(error, glDisable(GL_BLEND));
}

void gl_blend_func(GLError* error, GLenum sfactor, GLenum dfactor) {
	GL_ASSERT(error, glBlendFunc(sfactor, dfactor));
}

void gl_depth_func(GLError* error, GLenum func) {
	GL_ASSERT(error, glDepthFunc(func));
}