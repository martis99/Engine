#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/amaterial.h"
#include "api/gfx/abuffer.h"
#include "gl_atypes.h"
#include "gl/gl_shader.h"

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
	case VEC1I: glUniform1iv(location, 1, data); break;
	case VEC2I: glUniform2iv(location, 1, data); break;
	case VEC3I: glUniform3iv(location, 1, data); break;
	case VEC4I: glUniform4iv(location, 1, data); break;
	case VEC1UI: glUniform1uiv(location, 1, data); break;
	case VEC2UI: glUniform2uiv(location, 1, data); break;
	case VEC3UI: glUniform3uiv(location, 1, data); break;
	case VEC4UI: glUniform4uiv(location, 1, data); break;
	case VEC1F: glUniform1fv(location, 1, data); break;
	case VEC2F: glUniform2fv(location, 1, data); break;
	case VEC3F: glUniform3fv(location, 1, data); break;
	case VEC4F: glUniform4fv(location, 1, data); break;
	case MAT4F: glUniformMatrix4fv(location, 1, GL_FALSE, data); break;
	default: break;
	}
}

void amaterial_bind(AMaterial* material, ARenderer* renderer, uint slot) {
	for (uint i = 0; i < material->buffer->count; i++) {
		upload_value(material->buffer->types[i], material->buffer->locations[i], (byte*)material->buffer->data + material->buffer->offsets[i]);
	}
}
#endif