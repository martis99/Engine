#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/auniform_buffer.h"

#include "gl/gl_uniform_buffer.h"

struct AUniformBuffer {
	GLuint ub;
};

AUniformBuffer* auniformbuffer_create() {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer) {
	gl_ub_delete(uniform_buffer->ub);
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_init_static(AUniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size) {
	uniform_buffer->ub = gl_ub_create();
	gl_ub_create_static(uniform_buffer->ub, uniforms, uniforms_size);
}

void auniformbuffer_init_dynamic(AUniformBuffer* uniform_buffer, uint uniforms_size) {
	uniform_buffer->ub = gl_ub_create();
	gl_ub_create_dynamic(uniform_buffer->ub, uniforms_size);
}

void auniformbuffer_bind_base(AUniformBuffer* uniform_buffer, uint index) {
	gl_ub_bind_base(uniform_buffer->ub, index);
}

void auniformbuffer_set_data(AUniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size) {
	gl_ub_set_data(uniform_buffer->ub, uniforms, uniforms_size);
}

#endif