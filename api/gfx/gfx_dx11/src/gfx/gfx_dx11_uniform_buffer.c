#include "api/gfx/gfx_api_uniform_buffer.h"
#include "dx11/dx11_buffer.h"

#include "gfx_dx11_types.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, uint slot, uint data_size, const void* data) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = dx11_cb_create_static(renderer->error, renderer->device, data, data_size);
	if (uniform_buffer->buffer == NULL) {
		log_msg(renderer->log, "Failed to create static constant buffer");
		return NULL;
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, uint slot, uint data_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = dx11_cb_create_dynamic(renderer->error, renderer->device, data_size);
	if (uniform_buffer->buffer == NULL) {
		log_msg(renderer->log, "Failed to create dynamic constant buffer");
		return NULL;
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	if (uniform_buffer->buffer != NULL) {
		dx11_cb_delete(uniform_buffer->buffer);
		uniform_buffer->buffer = NULL;
	}
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_upload(AUniformBuffer* uniform_buffer, ARenderer* renderer, const void* data, uint data_size) {
	dx11_cb_set_data(renderer->error, uniform_buffer->buffer, renderer->context, data, data_size);
}

void auniformbuffer_bind_vs(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	dx11_cb_bind_vs(uniform_buffer->buffer, renderer->context, uniform_buffer->slot);
}

void auniformbuffer_bind_ps(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	dx11_cb_bind_ps(uniform_buffer->buffer, renderer->context, uniform_buffer->slot);
}