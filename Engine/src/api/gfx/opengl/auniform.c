#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/auniform.h"

#include "gl/gl_program.h"

struct AShader {
	GLuint program;
};

struct AUniform {
	char* name;
	GLint location;
	AUniformType type;
	GLsizei count;
	void* data;
};

static size_t get_size(AUniformType type, GLsizei count) {
	size_t size;
	switch (type) {
	case EMPTY: size = 0; break;
	case VEC1I: size = 1 * sizeof(int); break;
	case VEC2I: size = 2 * sizeof(int); break;
	case VEC3I: size = 3 * sizeof(int); break;
	case VEC4I: size = 4 * sizeof(int); break;
	case VEC1F: size = 1 * sizeof(float); break;
	case VEC2F: size = 2 * sizeof(float); break;
	case VEC3F: size = 3 * sizeof(float); break;
	case VEC4F: size = 4 * sizeof(float); break;
	case MAT4F: size = 16 * sizeof(float); break;
	default: size = 0; break;
	}
	return size * count;
}

AUniform* auniform_create(AShader* shader, const char* name, AUniformType type, int count) {
	AUniform* uniform = m_malloc(sizeof(AUniform));

	uniform->name = m_malloc(strlen(name) + 1);
	memcpy(uniform->name, name, strlen(name) + 1);
	uniform->location = gl_program_get_uniform_location(shader->program, name);
	uniform->type = type;
	uniform->count = count;
	uniform->data = m_malloc(get_size(type, count));
	return uniform;
}

void auniform_delete(AUniform* uniform) {
	m_free(uniform->name, strlen(uniform->name) + 1);
	m_free(uniform->data, get_size(uniform->type, uniform->count));
	m_free(uniform, sizeof(AUniform));
}

void auniform_set(AUniform* uniform, const void* data) {
	memcpy(uniform->data, data, get_size(uniform->type, uniform->count));
}

void* auniform_get(AUniform* uniform) {
	return uniform->data;
}

size_t auniform_size(AUniform* uniform) {
	return get_size(uniform->type, uniform->count);
}

void auniform_upload(AUniform* uniform) {
	switch (uniform->type) {
	case EMPTY: break;
	case VEC1I: gl_uniform_vec1i(uniform->location, uniform->count, uniform->data); break;
	case VEC2I: gl_uniform_vec2i(uniform->location, uniform->count, uniform->data); break;
	case VEC3I: gl_uniform_vec3i(uniform->location, uniform->count, uniform->data); break;
	case VEC4I: gl_uniform_vec4i(uniform->location, uniform->count, uniform->data); break;
	case VEC1F: gl_uniform_vec1f(uniform->location, uniform->count, uniform->data); break;
	case VEC2F: gl_uniform_vec2f(uniform->location, uniform->count, uniform->data); break;
	case VEC3F: gl_uniform_vec3f(uniform->location, uniform->count, uniform->data); break;
	case VEC4F: gl_uniform_vec4f(uniform->location, uniform->count, uniform->data); break;
	case MAT4F: gl_uniform_mat4f(uniform->location, uniform->count, uniform->data); break;
	default: break;
	}
}

void auniform_upload_vec1i(AUniform* uniform, int count, const void* data) {
	gl_uniform_vec1i(uniform->location, count, data);
}

void auniform_upload_mat4f(AUniform* uniform, int count, const void* data) {
	gl_uniform_mat4f(uniform->location, count, data);
}
#endif