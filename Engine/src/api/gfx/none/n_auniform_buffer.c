#include "pch.h"
#ifdef GAPI_NONE
#include "api/gfx/auniform_buffer.h"
#include "n_atypes.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, AValue* values, uint values_size, const void* data) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, AValue* values, uint values_size) {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer) {
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_set_value(AUniformBuffer* uniform_buffer, uint index, const void* value) {

}

void auniformbuffer_upload(AUniformBuffer* uniform_buffer, ARenderer* renderer) {

}

void auniformbuffer_bind(AUniformBuffer* uniform_buffer, ARenderer* renderer, uint slot) {

}

#endif