#include "pch.h"
#include "uniform_buffer.h"

UniformBuffer* uniformbuffer_create_static(UniformBuffer* uniform_buffer, Renderer* renderer, const void* uniforms, uint uniforms_size) {
	uniform_buffer->uniform_buffer = auniformbuffer_create_static(renderer->renderer, uniforms, uniforms_size);
	return uniform_buffer;
}

UniformBuffer* uniformbuffer_create_dynamic(UniformBuffer* uniform_buffer, Renderer* renderer, uint uniforms_size) {
	uniform_buffer->uniform_buffer = auniformbuffer_create_dynamic(renderer->renderer, uniforms_size);
	return uniform_buffer;
}

void uniformbuffer_delete(UniformBuffer* uniform_buffer) {
	auniformbuffer_delete(uniform_buffer->uniform_buffer);
}

void uniformbuffer_bind_base(UniformBuffer* uniform_buffer, Renderer* renderer, uint index) {
	auniformbuffer_bind_base(uniform_buffer->uniform_buffer, renderer->renderer, index);
}

void uniformbuffer_set_data(UniformBuffer* uniform_buffer, Renderer* renderer, const void* uniforms, uint uniforms_size) {
	auniformbuffer_set_data(uniform_buffer->uniform_buffer, renderer->renderer, uniforms, uniforms_size);
}