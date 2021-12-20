#include "pch.h"
#include "api/gfx/ashader.h"
#include "gn_atypes.h"

AShader* ashader_create(ARenderer* renderer, const char* src_vert, const char* src_frag, const char* textures, uint num_textures) {
	AShader* shader = m_malloc(sizeof(AShader));
	return shader;
}

void ashader_delete(AShader* shader) {
	m_free(shader, sizeof(AShader));
}

void ashader_bind(AShader* shader, ARenderer* renderer) {

}

void ashader_bind_uniform_block(AShader* shader, const char* name, uint index) {

}