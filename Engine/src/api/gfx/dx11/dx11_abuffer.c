#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/abuffer.h"
#include "dx11_atypes.h"

ABuffer* abuffer_create(AValue* values, uint values_size, AShader* shader) {
	ABuffer* buffer = m_malloc(sizeof(ABuffer));

	buffer->count = values_size / sizeof(AValue);
	buffer->offsets = m_malloc(buffer->count * sizeof(UINT));
	buffer->sizes = m_malloc(buffer->count * sizeof(UINT));
	buffer->size = 0;
	for (UINT i = 0; i < buffer->count; i++) {
		buffer->offsets[i] = buffer->size;
		buffer->sizes[i] = atype_size(values[i].type);
		buffer->size += buffer->sizes[i];
	}
	buffer->size = (uint)(ceilf(buffer->size / 16.0f) * 16);
	buffer->data = m_malloc(buffer->size);

	return buffer;
}

void abuffer_delete(ABuffer* buffer) {
	m_free(buffer->offsets, buffer->count * sizeof(UINT));
	m_free(buffer->sizes, buffer->count * sizeof(UINT));
	m_free(buffer->data, buffer->size);
	m_free(buffer, sizeof(ABuffer));
}

void abuffer_set_value(ABuffer* uniform_buffer, uint index, const void* value) {
	memcpy((byte*)uniform_buffer->data + uniform_buffer->offsets[index], value, uniform_buffer->sizes[index]);
}

#endif