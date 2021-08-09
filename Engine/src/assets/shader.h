#pragma once
#include "types.h"

Shader* shader_create(Shader* shader, Renderer* renderer, const char* src_vert, const char* src_frag, AShaderDesc desc);
void shader_delete(Shader* shader);

void shader_bind(Shader* shader, Renderer* renderer);