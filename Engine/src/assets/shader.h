#pragma once
#include "api/gfx/ashader.h"

typedef struct Shader {
	AShader* shader;
} Shader;

Shader* shader_create(Shader* shader, const char* src_vert, const char* src_frag);
void shader_delete(Shader* shader);

void shader_bind(Shader* shader);