#include "api/gfx/ashader.h"
#include "dx11_atypes.h"
#include "dx11/dx11_shader.h"

AShader* ashader_create(ARenderer* renderer, const char* src_vert, const char* src_frag, const char* textures, uint num_textures) {
	AShader* shader = m_malloc(sizeof(AShader));
	shader->vs = dx11_vs_create(renderer->device, src_vert, &shader->vs_blob);
	if (shader->vs == NULL) {
		log_error("Failed to create vertex shader");
		return NULL;
	}

	shader->ps = dx11_ps_create(renderer->device, src_frag, &shader->ps_blob);
	if (shader->ps == NULL) {
		log_error("Failed to create pixel shader");
		return NULL;
	}
	return shader;
}

void ashader_delete(AShader* shader) {
	if (shader->vs != NULL && shader->vs_blob != NULL) {
		dx11_vs_delete(shader->vs, shader->vs_blob);
		shader->vs = NULL;
		shader->vs_blob = NULL;
	}
	if (shader->ps != NULL && shader->ps_blob != NULL) {
		dx11_ps_delete(shader->ps, shader->ps_blob);
		shader->ps = NULL;
		shader->ps_blob = NULL;
	}
	m_free(shader, sizeof(AShader));
}

void ashader_bind(AShader* shader, ARenderer* renderer) {
	dx11_vs_bind(shader->vs, renderer->context);
	dx11_ps_bind(shader->ps, renderer->context);
}