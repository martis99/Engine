#include "api/gfx/auniform_buffer.h"
#include "gl_atypes.h"
#include "gl/gl_buffer.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, uint slot, uint data_size, const void* data) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = gl_ub_create_static(renderer->error, data, data_size);
	if (uniform_buffer->buffer == 0) {
		log_msg(renderer->log, "Failed to create uniform buffer");
		return NULL;
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, uint slot, uint data_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer = gl_ub_create_dynamic(renderer->error, data_size);
	if (uniform_buffer->buffer == 0) {
		log_msg(renderer->log, "Failed to create uniform buffer");
		return NULL;
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	if (uniform_buffer->buffer != 0) {
		gl_ub_delete(renderer->error, uniform_buffer->buffer);
		uniform_buffer->buffer = 0;
	}
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_upload(AUniformBuffer* uniform_buffer, ARenderer* renderer, const void* data, uint data_size) {
	gl_ub_set_data(renderer->error, uniform_buffer->buffer, data, data_size);
}

void auniformbuffer_bind_vs(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	gl_ub_bind_base(renderer->error, uniform_buffer->buffer, uniform_buffer->slot);
}

void auniformbuffer_bind_ps(AUniformBuffer* uniform_buffer, ARenderer* renderer) {
	gl_ub_bind_base(renderer->error, uniform_buffer->buffer, uniform_buffer->slot);
}