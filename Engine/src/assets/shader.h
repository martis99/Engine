#pragma once
#include "structs.h"

Shader* shader_create(Shader* shader, const char* src_vert, const char* src_frag, Renderer* renderer);
void shader_delete(Shader* shader);

void shader_bind(Shader* shader, Renderer* renderer);
void shader_set_model(Shader* shader, mat4* model);
void shader_set_entity(Shader* shader, int entity);

void shader_bind_uniform_block(Shader* shader, const char* name, uint index);