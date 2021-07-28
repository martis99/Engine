#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/auniform_buffer.h"
#include "dx11_astructs.h"
#include "dx11/dx11_buffer.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, const void* uniforms, uint uniforms_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->cb = dx11_cb_create_static(renderer->device, uniforms, uniforms_size);
	return uniform_buffer;
}

AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, uint uniforms_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->cb = dx11_cb_create_dynamic(renderer->device, uniforms_size);
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer) {
	dx11_cb_delete(uniform_buffer->cb);
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_bind_base(AUniformBuffer* uniform_buffer, ARenderer* renderer, uint index) {
	dx11_cb_bind_vs(uniform_buffer->cb, renderer->context, index);
}

void auniformbuffer_set_data(AUniformBuffer* uniform_buffer, ARenderer* renderer, const void* uniforms, uint uniforms_size) {
	dx11_cb_set_data(uniform_buffer->cb, renderer->context, uniforms, uniforms_size);
}
#endif