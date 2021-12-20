#pragma once
#include "api/atypes.h"

#include "gl/gl_types.h"

#include <Windows.h>

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

typedef struct GLAttachment {
	GLenum format;
	GLenum type;
	GLuint texture;
	GLenum target;
	GLint slot;
} GLAttachment;

struct AFramebuffer {
	int height;

	GLuint fb;
	GLAttachment** attachments;
	uint attachments_count;

	GLuint depth_stencil;

	AShader* shader;
	AMesh* mesh;
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

struct AUniformBuffer {
	GLuint buffer;
	GLuint slot;
};

GLenum gl_aprimitive(APrimitive primitive);
GLenum gl_ashadertype(AShaderType type);
GLint gl_awrap(AWrap wrap);
GLint gl_afilter(AFilter filter);
GLenum gl_afactor(AFactor factor);
GLenum gl_atype_type(AType type);
GLint gl_atype_internal_format(AType type);
GLenum gl_atype_format(AType type);