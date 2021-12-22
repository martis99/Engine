#include "pch.h"
#include "uniform_buffer.h"
#include "buffer.h"
#include "api/gfx/auniform_buffer.h"

UniformBuffer* uniformbuffer_create_static(UniformBuffer* uniform_buffer, Renderer* renderer, ABufferDesc* desc, const void* data) {
	buffer_create(&uniform_buffer->values, desc->values, desc->values_size, data);
	uniform_buffer->buffer = auniformbuffer_create_static(renderer->renderer, desc->slot, uniform_buffer->values.size, uniform_buffer->values.data);
	if (uniform_buffer->buffer == NULL) {
		return NULL;
	}
	return uniform_buffer;
}

UniformBuffer* uniformbuffer_create_dynamic(UniformBuffer* uniform_buffer, Renderer* renderer, ABufferDesc* desc) {
	buffer_create(&uniform_buffer->values, desc->values, desc->values_size, NULL);
	uniform_buffer->buffer = auniformbuffer_create_dynamic(renderer->renderer, desc->slot, uniform_buffer->values.size);
	if (uniform_buffer->buffer == NULL) {
		return NULL;
	}
	return uniform_buffer;
}

void uniformbuffer_delete(UniformBuffer* uniform_buffer) {
	buffer_delete(&uniform_buffer->values);
	auniformbuffer_delete(uniform_buffer->buffer);
}

void uniformbuffer_set_value(UniformBuffer* uniform_buffer, uint index, const void* value) {
	buffer_set_value(&uniform_buffer->values, index, value);
}

void uniformbuffer_upload(UniformBuffer* uniform_buffer, Renderer* renderer) {
	auniformbuffer_upload(uniform_buffer->buffer, renderer->renderer, uniform_buffer->values.data, uniform_buffer->values.size);
}

void uniformbuffer_bind_vs(UniformBuffer* uniform_buffer, Renderer* renderer) {
	auniformbuffer_bind_vs(uniform_buffer->buffer, renderer->renderer);
}

void uniformbuffer_bind_ps(UniformBuffer* uniform_buffer, Renderer* renderer) {
	auniformbuffer_bind_ps(uniform_buffer->buffer, renderer->renderer);
}