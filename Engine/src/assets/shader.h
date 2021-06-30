#pragma once
#include "api/gfx/ashader.h"

#include "uniform.h"

typedef struct Shader {
	AShader* shader;
	Uniform view_projection;
	Uniform model;
} Shader;

Shader* shader_create(Shader* shader, const char* src_vert, const char* src_frag);
void shader_delete(Shader* shader);

void shader_bind(Shader* shader, mat4* view_projection);
void shader_set_model(Shader* shader, mat4* model);