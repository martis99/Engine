#pragma once
#include "gfx_types.h"

Shader *shader_create(Shader *shader, Renderer *renderer, const char *vert, const char *frag, AShaderDesc desc);
void shader_delete(Shader *shader, Renderer *renderer);

void shader_bind(Shader *shader, Renderer *renderer);
