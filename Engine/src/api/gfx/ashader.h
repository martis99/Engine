#pragma once
#include "api/astructs.h"

AShader* ashader_create(ARenderer* renderer, const char* src_vert, const char* src_frag, const char* textures, uint num_textures);
void ashader_delete(AShader* shader);

void ashader_bind(AShader* shader, ARenderer* renderer);

void ashader_bind_uniform_block(AShader* shader, const char* name, uint index);