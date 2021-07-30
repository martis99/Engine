#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/amaterial.h"
#include "gl_astructs.h"
#include "api/gfx/abuffer.h"
#include "gl/gl_uniform_buffer.h"
#include "gl/gl_program.h"

AMaterial* amaterial_create(ARenderer* renderer, AShader* shader, AValue* values, uint values_size) {
	AMaterial* material = m_malloc(sizeof(AMaterial));
	material->buffer = abuffer_create(values, values_size, shader);
	return material;
}

void amaterial_delete(AMaterial* material) {
	abuffer_delete(material->buffer);
	m_free(material, sizeof(AMaterial));
}

void amaterial_set_value(AMaterial* material, uint index, const void* value) {
	abuffer_set_value(material->buffer, index, value);
}

void amaterial_upload(AMaterial* material, ARenderer* renderer) {

}

void upload_value(AType type, GLint location, const void* data) {
	switch (type) {
	case EMPTY: break;
	case VEC1I: gl_uniform_vec1i(location, 1, data); break;
	case VEC2I: gl_uniform_vec2i(location, 1, data); break;
	case VEC3I: gl_uniform_vec3i(location, 1, data); break;
	case VEC4I: gl_uniform_vec4i(location, 1, data); break;
	case VEC1F: gl_uniform_vec1f(location, 1, data); break;
	case VEC2F: gl_uniform_vec2f(location, 1, data); break;
	case VEC3F: gl_uniform_vec3f(location, 1, data); break;
	case VEC4F: gl_uniform_vec4f(location, 1, data); break;
	case MAT4F: gl_uniform_mat4f(location, 1, data); break;
	default: break;
	}
}

void amaterial_bind(AMaterial* material, ARenderer* renderer, uint slot) {
	for (uint i = 0; i < material->buffer->count; i++) {
		upload_value(material->buffer->types[i], material->buffer->locations[i], (byte*)material->buffer->data + material->buffer->offsets[i]);
	}
}
#endif