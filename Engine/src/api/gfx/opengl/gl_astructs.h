#pragma once
#ifdef GAPI_OPENGL
#include "api/aenums.h"

#include "gl/gl.h"

struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
	AWindowCallbacks callbacks;
	ACursor* cursor;
};

struct AContext {
	HWND window;
	HDC device;
	HGLRC context;
	HMODULE library;
};

struct AFramebuffer {
	GLuint fb;
	GLuint* attachments;
	GLsizei attachments_count;
};

struct AMesh {
	GLuint va;
	GLuint vb;
	GLuint ivb;
	GLuint ib;
	GLsizei count;

	APrimitive primitive;

	GLuint layout_index;
};

struct ARenderer {
	byte id;
};

struct AShader {
	GLuint program;
};

struct ATexture {
	GLuint id;
};

struct AUniform {
	char* name;
	GLint location;
	ADataType type;
	GLsizei count;
	void* data;
};

struct AUniformBuffer {
	GLuint ub;
};
#endif