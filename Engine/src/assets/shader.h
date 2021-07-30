#pragma once
#include "structs.h"

Shader* shader_create(Shader* shader, Renderer* renderer, const char* src_vert, const char* src_frag, AValue* layout, uint layout_size, AValue* props, uint props_size, const char* textures, uint num_textures);
void shader_delete(Shader* shader);

void shader_bind(Shader* shader, Renderer* renderer);

void shader_bind_uniform_block(Shader* shader, const char* name, uint index);