#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/auniform_buffer.h"
#include "gl_astructs.h"
#include "api/gfx/abuffer.h"
#include "gl/gl_uniform_buffer.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, AValue* values, uint values_size, const void* data) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = abuffer_create(values, values_size, NULL);
	uniform_buffer->ub = gl_ub_create();
	gl_ub_create_static(uniform_buffer->ub, uniform_buffer->buffer->data, uniform_buffer->buffer->size);
	return uniform_buffer;
}

AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, AValue* values, uint values_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = abuffer_create(values, values_size, NULL);
	uniform_buffer->ub = gl_ub_create();
	gl_ub_create_dynamic(uniform_buffer->ub, uniform_buffer->buffer->size);
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer) {
	abuffer_delete(uniform_buffer->buffer);
	gl_ub_delete(uniform_buffer->ub);
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_set_value(AUniformBuffer* uniform_buffer, uint index, const void* value) {
	abuffer_set_value(uniform_buffer->buffer, index, value);
}

void auniformbuffer_upload(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	gl_ub_set_data(uniform_buffer->ub, uniform_buffer->buffer->data, uniform_buffer->buffer->size);
}

void auniformbuffer_bind(AUniformBuffer* uniform_buffer, ARenderer* renderer, uint slot) {
	gl_ub_bind_base(uniform_buffer->ub, slot);
}
#endif