#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/auniform_buffer.h"
#include "gl_astructs.h"

#include "gl/gl_uniform_buffer.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, const void* uniforms, uint uniforms_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->ub = gl_ub_create();
	gl_ub_create_static(uniform_buffer->ub, uniforms, uniforms_size);
	return uniform_buffer;
}

AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, uint uniforms_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->ub = gl_ub_create();
	gl_ub_create_dynamic(uniform_buffer->ub, uniforms_size);
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer) {
	gl_ub_delete(uniform_buffer->ub);
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_bind_base(AUniformBuffer* uniform_buffer, ARenderer* renderer, uint index) {
	gl_ub_bind_base(uniform_buffer->ub, index);
}

void auniformbuffer_set_data(AUniformBuffer* uniform_buffer, ARenderer* renderer, const void* uniforms, uint uniforms_size) {
	gl_ub_set_data(uniform_buffer->ub, uniforms, uniforms_size);
}

#endif