#include "pch.h"
#include "uniform_buffer.h"

UniformBuffer* uniformbuffer_create(UniformBuffer* uniform_buffer) {
	uniform_buffer->uniform_buffer = auniformbuffer_create();
	return uniform_buffer;
}

void uniformbuffer_delete(UniformBuffer* uniform_buffer) {
	auniformbuffer_delete(uniform_buffer->uniform_buffer);
}

void uniformbuffer_init_static(UniformBuffer* uniform_buffer, const void* data, uint size) {
	auniformbuffer_init_static(uniform_buffer->uniform_buffer, data, size);
}

void uniformbuffer_init_dynamic(UniformBuffer* uniform_buffer, uint size) {
	auniformbuffer_init_dynamic(uniform_buffer->uniform_buffer, size);
}

void uniformbuffer_bind_base(UniformBuffer* uniform_buffer, uint index) {
	auniformbuffer_bind_base(uniform_buffer->uniform_buffer, index);
}

void uniformbuffer_set_data(UniformBuffer* uniform_buffer, const void* data, uint size) {
	auniformbuffer_set_data(uniform_buffer->uniform_buffer, data, size);
}