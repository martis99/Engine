#include "pch.h"
#include "uniform_buffer.h"
#include "api/gfx/auniform_buffer.h"

UniformBuffer* uniformbuffer_create_static(UniformBuffer* uniform_buffer, Renderer* renderer, AValue* uniforms, uint uniforms_size, const void* data) {
	uniform_buffer->uniform_buffer = auniformbuffer_create_static(renderer->renderer, uniforms, uniforms_size, data);
	return uniform_buffer;
}

UniformBuffer* uniformbuffer_create_dynamic(UniformBuffer* uniform_buffer, Renderer* renderer, AValue* uniforms, uint uniforms_size) {
	uniform_buffer->uniform_buffer = auniformbuffer_create_dynamic(renderer->renderer, uniforms, uniforms_size);
	return uniform_buffer;
}

void uniformbuffer_delete(UniformBuffer* uniform_buffer) {
	auniformbuffer_delete(uniform_buffer->uniform_buffer);
}

void uniformbuffer_set_value(UniformBuffer* uniform_buffer, uint index, const void* value) {
	auniformbuffer_set_value(uniform_buffer->uniform_buffer, index, value);
}

void uniformbuffer_upload(UniformBuffer* uniform_buffer, Renderer* renderer) {
	auniformbuffer_upload(uniform_buffer->uniform_buffer, renderer->renderer);
}

void uniformbuffer_bind(UniformBuffer* uniform_buffer, Renderer* renderer, uint slot) {
	auniformbuffer_bind(uniform_buffer->uniform_buffer, renderer->renderer, slot);
}