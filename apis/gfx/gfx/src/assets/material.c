#include "pch.h"
#include "material.h"
#include "uniform_buffer.h"

#include "texture.h"

Material* material_create(Material* material, Renderer* renderer, Shader* shader) {
	material->shader = shader;

	ABufferDesc* vs_desc = ashaderdesc_get_bufferdesc(shader->desc, A_BFR_VS);
	ABufferDesc* ps_desc = ashaderdesc_get_bufferdesc(shader->desc, A_BFR_PS);

	if (vs_desc != NULL) {
		material->vs = 1;
		if (uniformbuffer_create_dynamic(&material->vs_buffer, renderer, vs_desc) == NULL) {
			log_error("Failed to create material vertex shader buffer");
			return NULL;
		}
	}

	if (ps_desc != NULL) {
		material->ps = 1;
		if (uniformbuffer_create_dynamic(&material->ps_buffer, renderer, ps_desc) == NULL) {
			log_error("Failed to create material pixel shader buffer");
		}
	}

	material->textures_cap = shader->desc.textures_count;
	if (material->textures_cap > 0) {
		material->textures = m_malloc(material->textures_cap * sizeof(Texture*));
	}
	material->textures_count = 0;

	for (uint i = 0; i < material->textures_cap; i++) {
		material->textures[i] = &shader->default_texture;
	}

	return material;
}

void material_delete(Material* material) {
	if (material->vs == 1) {
		uniformbuffer_delete(&material->vs_buffer);
	}
	if (material->ps == 1) {
		uniformbuffer_delete(&material->ps_buffer);
	}
	if (material->textures_cap > 0) {
		m_free(material->textures, material->textures_cap * sizeof(Texture*));
	}
}

void material_set_vs_value(Material* material, uint index, const void* value) {
	if (material->vs == 0) {
		return;
	}
	uniformbuffer_set_value(&material->vs_buffer, index, value);
}

void material_set_ps_value(Material* material, uint index, const void* value) {
	if (material->ps == 0) {
		return;
	}
	uniformbuffer_set_value(&material->ps_buffer, index, value);
}

void material_upload(Material* material, Renderer* renderer) {
	if (material->vs == 1) {
		uniformbuffer_upload(&material->vs_buffer, renderer);
	}
	if (material->ps == 1) {
		uniformbuffer_upload(&material->ps_buffer, renderer);
	}
}

uint material_add_texture(Material* material, Texture* texture) {
	for (uint i = 0; i < material->textures_count; i++) {
		if (material->textures[i] == texture) {
			return i;
		}
	}

	if (material->textures_cap > material->textures_count) {
		material->textures[material->textures_count++] = texture;
	}
	return material->textures_count - 1;
}

void material_bind(Material* material, Renderer* renderer) {
	if (material->vs == 1) {
		uniformbuffer_bind_vs(&material->vs_buffer, renderer);
	}
	if (material->ps == 1) {
		uniformbuffer_bind_ps(&material->ps_buffer, renderer);
	}

	for (uint i = 0; i < material->textures_cap; i++) {
		texture_bind(material->textures[i], renderer, i);
	}
}