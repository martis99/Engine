#include "pch.h"
#include "material.h"
#include "api/gfx/amaterial.h"

#include "texture.h"

Material* material_create(Material* material, Renderer* renderer, Shader* shader) {
	material->shader = shader;
	material->textures_cap = shader->num_textures;
	material->material = amaterial_create(renderer->renderer, shader->shader, shader->props, shader->props_size);
	material->textures = m_malloc(material->textures_cap * sizeof(Texture*));
	material->textures_count = 0;

	for (uint i = 0; i < material->textures_cap; i++) {
		material->textures[i] = &shader->default_texture;
	}

	return material;
}

void material_delete(Material* material) {
	amaterial_delete(material->material);
	m_free(material->textures, material->textures_cap * sizeof(Texture*));
}

void material_set_value(Material* material, uint index, const void* value) {
	amaterial_set_value(material->material, index, value);
}

void material_upload(Material* material, Renderer* renderer) {
	amaterial_upload(material->material, renderer->renderer);
}

uint material_add_texture(Material* material, Texture* texture) {
	for (uint i = 0; i < material->textures_count; i++) {
		if (material->textures[i] == texture) {
			return i;
		}
	}

	material->textures[material->textures_count++] = texture;
	return material->textures_count - 1;
}

void material_bind(Material* material, Renderer* renderer, uint slot) {
	amaterial_bind(material->material, renderer->renderer, slot);

	for (uint i = 0; i < material->textures_cap; i++) {
		texture_bind(material->textures[i], renderer, i);
	}
}