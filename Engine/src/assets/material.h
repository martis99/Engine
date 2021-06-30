#pragma once
#include "shader.h"

typedef struct Material {
	Shader* shader;
	Dictionary* uniforms;
} Material;

Material* material_create(Material* material, Shader* shader);
void material_delete(Material* material);

void material_bind(Material* material);

void material_set_vec4f(Material* material, const char* name, int count, vec4* value);