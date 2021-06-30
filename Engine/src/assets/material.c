#include "pch.h"
#include "material.h"
#include "uniform.h"

#define MAX_TEXTURES 16

Material* material_create(Material* material, Shader* shader) {
	material->shader = shader;
	material->uniforms = dic_create(20, sizeof(Uniform));
	material->textures = m_malloc(MAX_TEXTURES * sizeof(Texture*));
	material->textures_count = 0;

	return material;
}

void material_delete(Material* material) {
	dic_delete(material->uniforms, uniform_delete);
	m_free(material->textures, MAX_TEXTURES * sizeof(Texture*));
}

void material_bind(Material* material) {
	dic_exec(material->uniforms, uniform_upload);

	for (uint i = 0; i < material->textures_count; i++) {
		texture_bind(material->textures[i], i);
	}
}

static void set_uniform(Material* material, const char* name, AUniformType type, int count, const void* data) {
	Uniform* uniform = uniform_create(dic_add(material->uniforms, name), material->shader, name, type, count);
	uniform_set(uniform, data);
}

void material_set_vec1i(Material* material, const char* name, int count, int* value) {
	set_uniform(material, name, VEC1I, count, value);
}

void material_set_vec4f(Material* material, const char* name, int count, vec4* value) {
	set_uniform(material, name, VEC4F, count, value);
}

static void add_texture(Material* material, Texture* texture) {
	for (uint i = 0; i < material->textures_count; i++) {
		if (material->textures[i] == texture) {
			return;
		}
	}

	material->textures[material->textures_count] = texture;
	material->textures_count++;
}

void material_add_texture(Material* material, Texture* texture) {
	add_texture(material, texture);
}