#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/auniform_buffer.h"
#include "api/gfx/abuffer.h"
#include "dx11_atypes.h"
#include "dx11/dx11_buffer.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, AValue* uniforms, uint uniforms_size, const void* data) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = abuffer_create(uniforms, uniforms_size, NULL);
	memcpy(uniform_buffer->buffer->data, data, uniform_buffer->buffer->size);
	uniform_buffer->cb = dx11_cb_create_static(renderer->device, uniform_buffer->buffer->data, uniform_buffer->buffer->size);
	if (uniform_buffer->cb == NULL) {
		log_error("Failed to create constant buffer");
		return NULL;
	}
	return uniform_buffer;
}

AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, AValue* uniforms, uint uniforms_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = abuffer_create(uniforms, uniforms_size, NULL);
	uniform_buffer->cb = dx11_cb_create_dynamic(renderer->device, uniform_buffer->buffer->size);
	if (uniform_buffer->cb == NULL) {
		log_error("Failed to create constant buffer");
		return NULL;
	}
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer) {
	if (uniform_buffer->cb != NULL) {
		dx11_cb_delete(uniform_buffer->cb);
	}
	abuffer_delete(uniform_buffer->buffer);
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_set_value(AUniformBuffer* uniform_buffer, uint index, const void* value) {
	abuffer_set_value(uniform_buffer->buffer, index, value);
}

void auniformbuffer_upload(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	dx11_cb_set_data(uniform_buffer->cb, renderer->context, uniform_buffer->buffer->data, uniform_buffer->buffer->size);
}

void auniformbuffer_bind(AUniformBuffer* uniform_buffer, ARenderer* renderer, uint slot) {
	dx11_cb_bind_vs(uniform_buffer->cb, renderer->context, slot);
}

#endif