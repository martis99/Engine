#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/amaterial.h"
#include "dx11_astructs.h"
#include "dx11/dx11_buffer.h"
#include "api/gfx/abuffer.h"

AMaterial* amaterial_create(ARenderer* renderer, AShader* shader, AValue* values, uint values_size) {
	AMaterial* material = m_malloc(sizeof(AMaterial));
	material->buffer = abuffer_create(values, values_size, shader);
	material->cb = dx11_cb_create_dynamic(renderer->device, material->buffer->size);
	return material;
}

void amaterial_delete(AMaterial* uniform_buffer) {
	dx11_cb_delete(uniform_buffer->cb);
	abuffer_delete(uniform_buffer->buffer);
	m_free(uniform_buffer, sizeof(AMaterial));
}

void amaterial_set_value(AMaterial* uniform_buffer, uint index, const void* value) {
	abuffer_set_value(uniform_buffer->buffer, index, value);
}

void amaterial_upload(AMaterial* uniform_buffer, ARenderer* renderer) {
	dx11_cb_set_data(uniform_buffer->cb, renderer->context, uniform_buffer->buffer->data, uniform_buffer->buffer->size);
}

void amaterial_bind(AMaterial* uniform_buffer, ARenderer* renderer, uint slot) {
	dx11_cb_bind_vs(uniform_buffer->cb, renderer->context, slot);
}

#endif