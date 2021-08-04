#pragma once
#ifdef GAPI_OPENGL
#include "api/atypes.h"

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

	uint vertex_size;
	GLuint vertices;
	GLsizei vertices_count;

	uint instance_size;
	GLuint instances;
	GLsizei instances_count;

	uint index_size;
	GLuint indices;
	GLsizei indices_count;
	GLenum index_type;

	GLenum primitive;
};

struct ARenderer {
	byte id;
};

struct AShader {
	GLuint program;
	GLint textures_location;
	GLuint num_textures;
	GLint* textures;
};

struct ATexture {
	GLuint id;
};

struct ABuffer {
	uint* offsets;
	uint* sizes;
	GLint* locations;
	AType* types;
	uint size;
	uint count;
	void* data;
};

struct AUniformBuffer {
	GLuint ub;
	ABuffer* buffer;
};

struct AMaterial {
	ABuffer* buffer;
};

GLenum gl_aprimitive(APrimitive primitive);
GLenum gl_ashadertype(AShaderType type);
GLint gl_awrap(AWrap wrap);
GLint gl_afilter(AFilter filter);
GLenum gl_afactor(AFactor factor);
GLenum gl_atype_type(AType type);
#endif