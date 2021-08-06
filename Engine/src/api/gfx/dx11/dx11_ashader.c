#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/ashader.h"
#include "dx11_atypes.h"
#include "dx11/dx11_shader.h"

AShader* ashader_create(ARenderer* renderer, const char* src_vert, const char* src_frag, const char* textures, uint num_textures) {
	AShader* shader = m_malloc(sizeof(AShader));
	shader->vs = dx11_vs_create(renderer->device, src_vert, &shader->vs_blob);
	if (shader->vs == NULL) {
		log_error("Failed to create shader");
		return NULL;
	}

	shader->ps = dx11_ps_create(renderer->device, src_frag, &shader->ps_blob);
	if (shader->ps == NULL) {
		log_error("Failed to create shader");
		return NULL;
	}
	return shader;
}

void ashader_delete(AShader* shader) {
	dx11_vs_delete(shader->vs, shader->vs_blob);
	dx11_ps_delete(shader->ps, shader->ps_blob);
	m_free(shader, sizeof(AShader));
}

void ashader_bind(AShader* shader, ARenderer* renderer) {
	dx11_vs_bind(shader->vs, renderer->context);
	dx11_ps_bind(shader->ps, renderer->context);
}

void ashader_bind_uniform_block(AShader* shader, const char* name, uint index) {

}

#endif