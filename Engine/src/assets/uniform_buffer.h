#pragma once
#include "structs.h"

UniformBuffer* uniformbuffer_create_static(UniformBuffer* uniform_buffer, Renderer* renderer, AValue* uniforms, uint uniforms_size, const void* data);
UniformBuffer* uniformbuffer_create_dynamic(UniformBuffer* uniform_buffer, Renderer* renderer, AValue* uniforms, uint uniforms_size);
void uniformbuffer_delete(UniformBuffer* uniform_buffer);

void uniformbuffer_set_value(UniformBuffer* uniform_buffer, uint index, const void* value);
void uniformbuffer_upload(UniformBuffer* uniform_buffer, Renderer* renderer);
void uniformbuffer_bind(UniformBuffer* uniform_buffer, Renderer* renderer, uint slot);