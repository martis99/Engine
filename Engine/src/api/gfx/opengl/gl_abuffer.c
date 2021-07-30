#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/abuffer.h"
#include "gl_astructs.h"

#include "gl/gl_program.h"

ABuffer* abuffer_create(AValue* values, uint values_size, AShader* shader) {
	ABuffer* buffer = m_malloc(sizeof(ABuffer));

	buffer->count = values_size / sizeof(AValue);
	buffer->offsets = m_malloc(buffer->count * sizeof(uint));
	buffer->sizes = m_malloc(buffer->count * sizeof(uint));
	buffer->locations = m_malloc(buffer->count * sizeof(GLint));
	buffer->types = m_malloc(buffer->count * sizeof(AType));
	buffer->size = 0;
	for (uint i = 0; i < buffer->count; i++) {
		buffer->offsets[i] = buffer->size;
		buffer->sizes[i] = type_size(values[i].type);
		buffer->types[i] = values[i].type;
		if (shader != NULL) {
			buffer->locations[i] = gl_program_get_uniform_location(shader->program, values[i].name);
		}
		buffer->size += buffer->sizes[i];
	}
	buffer->data = m_malloc(buffer->size);

	return buffer;
}

void abuffer_delete(ABuffer* buffer) {
	m_free(buffer->offsets, buffer->count * sizeof(uint));
	m_free(buffer->sizes, buffer->count * sizeof(uint));
	m_free(buffer->locations, buffer->count * sizeof(GLint));
	m_free(buffer->types, buffer->count * sizeof(AType));
	m_free(buffer->data, buffer->size);
	m_free(buffer, sizeof(ABuffer));
}

void abuffer_set_value(ABuffer* buffer, uint index, const void* value) {
	memcpy((byte*)buffer->data + buffer->offsets[index], value, buffer->sizes[index]);
}

#endif