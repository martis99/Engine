#pragma once
#include "structs.h"

UniformBuffer* uniformbuffer_create_static(UniformBuffer* uniform_buffer, Renderer* renderer, const void* uniforms, uint uniforms_size);
UniformBuffer* uniformbuffer_create_dynamic(UniformBuffer* uniform_buffer, Renderer* renderer, uint uniforms_size);
void uniformbuffer_delete(UniformBuffer* uniform_buffer);

void uniformbuffer_bind_base(UniformBuffer* uniform_buffer, Renderer* renderer, uint index);
void uniformbuffer_set_data(UniformBuffer* uniform_buffer, Renderer* renderer, const void* uniforms, uint uniforms_size);