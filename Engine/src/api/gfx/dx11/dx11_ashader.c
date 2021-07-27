#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/ashader.h"

struct AShader {
	uint id;
};

AShader* ashader_create(const char* src_vert, const char* src_frag) {
	AShader* shader = m_malloc(sizeof(AShader));
	return shader;
}

void ashader_delete(AShader* shader) {
	m_free(shader, sizeof(AShader));
}

void ashader_bind(AShader* shader) {

}

void ashader_bind_uniform_block(AShader* shader, const char* name, uint index) {

}

#endif