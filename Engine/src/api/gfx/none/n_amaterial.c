#include "pch.h"
#ifdef GAPI_NONE
#include "api/gfx/amaterial.h"
#include "n_atypes.h"

AMaterial* amaterial_create(ARenderer* renderer, AShader* shader, AValue* values, uint values_size) {
	AMaterial* material = m_malloc(sizeof(AMaterial));
	return material;
}

void amaterial_delete(AMaterial* material) {
	m_free(material, sizeof(AMaterial));
}

void amaterial_set_value(AMaterial* material, uint index, const void* value) {

}

void amaterial_upload(AMaterial* material, ARenderer* renderer) {

}

void amaterial_bind(AMaterial* material, ARenderer* renderer, uint slot) {

}

#endif