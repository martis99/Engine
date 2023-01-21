#pragma once
#include "gfx_types.h"

ShaderCreator *gfx_sc_create(ShaderCreator *shader_creator, Renderer *renderer);
void gfx_sc_delete(ShaderCreator *shader_creator);

Shader *gfx_sc_create_shader(ShaderCreator *shader_creator, Shader *shader, Renderer *renderer, const char *vert, const char *frag, AShaderDesc desc);
