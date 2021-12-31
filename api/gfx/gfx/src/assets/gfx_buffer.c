#include "gfx_buffer.h"

ABuffer* buffer_create(ABuffer* buffer, AValue* values, uint values_size, const void* data) {
	buffer->count = values_size / sizeof(AValue);
	buffer->offsets = m_malloc(buffer->count * sizeof(uint));
	buffer->sizes = m_malloc(buffer->count * sizeof(uint));
	buffer->size = 0;
	for (uint i = 0; i < buffer->count; i++) {
		buffer->offsets[i] = buffer->size;
		buffer->sizes[i] = atype_size(values[i].type);
		buffer->size += buffer->sizes[i];
	}
	buffer->size = (uint)(ceilf(buffer->size / 16.0f) * 16);
	buffer->data = m_malloc(buffer->size);
	if (data != NULL) {
		memcpy(buffer->data, data, buffer->size);
	}
	return buffer;
}

void buffer_delete(ABuffer* buffer) {
	m_free(buffer->offsets, buffer->count * sizeof(uint));
	m_free(buffer->sizes, buffer->count * sizeof(uint));
	m_free(buffer->data, buffer->size);
}

void buffer_set_value(ABuffer* buffer, uint index, const void* value) {
	int s = buffer->sizes[index];
	memcpy((byte*)buffer->data + buffer->offsets[index], value, buffer->sizes[index]);
}