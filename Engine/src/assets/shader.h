#pragma once
#include "api/gfx/ashader.h"

#include "uniform.h"

typedef struct Shader {
	AShader* shader;
	Uniform model;
	Uniform entity;
} Shader;

Shader* shader_create(Shader* shader, const char* src_vert, const char* src_frag);
void shader_delete(Shader* shader);

void shader_bind(Shader* shader);
void shader_set_model(Shader* shader, mat4* model);
void shader_set_entity(Shader* shader, int entity);

void shader_bind_uniform_block(Shader* shader, const char* name, uint index);