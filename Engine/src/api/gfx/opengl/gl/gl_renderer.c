#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_renderer.h"

void gl_cull_face_enable() {
	glEnable(GL_CULL_FACE);
}

void gl_cull_face_disable() {
	glDisable(GL_CULL_FACE);
}

void gl_depth_test_enable() {
	glEnable(GL_DEPTH_TEST);
}

void gl_depth_test_disable() {
	glDisable(GL_DEPTH_TEST);
}

void gl_polygon_mode_fill() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void gl_polygon_mode_line() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void gl_cull_face_back() {
	glCullFace(GL_BACK);
}

void gl_front_face_cw() {
	glFrontFace(GL_CW);
}

void gl_unpack_alignment() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void gl_blend_enable() {
	glEnable(GL_BLEND);
}

void gl_blend_disable() {
	glDisable(GL_BLEND);
}

void gl_blend_func(GLenum sfactor, GLenum dfactor) {
	glBlendFunc(sfactor, dfactor);
}

static void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {

	printf("---------------------opengl-callback-start------------\n");
	printf("message: %s\n", message);
	printf("type: ");
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		printf("ERROR");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		printf("DEPRECATED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		printf("UNDEFINED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		printf("PORTABILITY");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		printf("PERFORMANCE");
		break;
	case GL_DEBUG_TYPE_OTHER:
		printf("OTHER");
		break;
	}

	printf("\nid: %i\n", id);
	printf("severity: ");
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		printf("LOW");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		printf("MEDIUM");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		printf("HIGH");
		break;
	}
	printf("\n---------------------opengl-callback-end--------------\n");
}

void gl_debug_output_enable() {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debug_message_callback, NULL);
	GLuint unusedIds = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, 1);
}

void gl_debug_output_disable() {
	glDisable(GL_DEBUG_OUTPUT);
	glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
#endif