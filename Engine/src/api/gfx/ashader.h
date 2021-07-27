#pragma once
#include "api/astructs.h"

AShader* ashader_create(const char* src_vert, const char* src_frag, ARenderer* renderer);
void ashader_delete(AShader* shader);

void ashader_bind(AShader* shader, ARenderer* renderer);

void ashader_bind_uniform_block(AShader* shader, const char* name, uint index);