#include "pch.h"
#ifdef GAPI_NONE
#include "api/gfx/abuffer.h"
#include "n_atypes.h"

ABuffer* abuffer_create(AValue* values, uint values_size, AShader* shader) {
	ABuffer* buffer = m_malloc(sizeof(ABuffer));
	return buffer;
}

void abuffer_delete(ABuffer* buffer) {
	m_free(buffer, sizeof(ABuffer));
}

void abuffer_set_value(ABuffer* buffer, uint index, const void* value) {

}

#endif