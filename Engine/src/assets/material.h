#pragma once
#include "shader.h"
#include "texture.h"

typedef struct Material {
	Shader* shader;
	Dictionary* uniforms;
	Texture** textures;
	uint textures_count;
} Material;

Material* material_create(Material* material, Shader* shader);
void material_delete(Material* material);

void material_bind(Material* material);

void material_set_vec1i(Material* material, const char* name, int count, int* value);
void material_set_vec4f(Material* material, const char* name, int count, vec4* value);
void material_add_texture(Material* material, Texture* texture);