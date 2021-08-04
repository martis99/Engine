#pragma once
#include "types.h"

Shader* shader_create(Shader* shader, Renderer* renderer, const char* src_vert, const char* src_frag, AMeshDesc mesh_desc, AValue* props, uint props_size, const char* textures, uint num_textures);
void shader_delete(Shader* shader);

void shader_bind(Shader* shader, Renderer* renderer);

void shader_bind_uniform_block(Shader* shader, const char* name, uint index);