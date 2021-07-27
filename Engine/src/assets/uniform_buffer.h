#pragma once
#include "structs.h"

UniformBuffer* uniformbuffer_create(UniformBuffer* uniform_buffer);
void uniformbuffer_delete(UniformBuffer* uniform_buffer);

void uniformbuffer_init_static(UniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size);
void uniformbuffer_init_dynamic(UniformBuffer* uniform_buffer, uint uniforms_size);
void uniformbuffer_bind_base(UniformBuffer* uniform_buffer, uint index);
void uniformbuffer_set_data(UniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size);