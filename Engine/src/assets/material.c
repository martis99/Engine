#include "pch.h"
#include "material.h"

#include "texture.h"
#include "api/gfx/amaterial.h"

#define MAX_TEXTURES 16

Material* material_create(Material* material, Renderer* renderer, Shader* shader) {
	material->shader = shader;
	material->material = amaterial_create(renderer->renderer, shader->shader, shader->props, shader->props_size);
	material->textures = m_malloc(MAX_TEXTURES * sizeof(Texture*));
	material->textures_count = 0;

	return material;
}

void material_delete(Material* material) {
	amaterial_delete(material->material);
	m_free(material->textures, MAX_TEXTURES * sizeof(Texture*));
}

void material_set_value(Material* material, uint index, const void* value) {
	amaterial_set_value(material->material, index, value);
}

void material_upload(Material* material, Renderer* renderer) {
	amaterial_upload(material->material, renderer->renderer);
}

void material_add_texture(Material* material, Texture* texture) {
	for (uint i = 0; i < material->textures_count; i++) {
		if (material->textures[i] == texture) {
			return;
		}
	}

	material->textures[material->textures_count++] = texture;
}

void material_bind(Material* material, Renderer* renderer, uint slot) {
	amaterial_bind(material->material, renderer->renderer, slot);

	for (uint i = 0; i < material->textures_count; i++) {
		texture_bind(material->textures[i], renderer, i);
	}
}