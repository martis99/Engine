#pragma once
#include "gfx_types.h"

UniformBuffer* uniformbuffer_create_static(UniformBuffer* uniform_buffer, Renderer* renderer, ABufferDesc* desc, const void* data);
UniformBuffer* uniformbuffer_create_dynamic(UniformBuffer* uniform_buffer, Renderer* renderer, ABufferDesc* desc);
void uniformbuffer_delete(UniformBuffer* uniform_buffer, Renderer* renderer);

void uniformbuffer_set_value(UniformBuffer* uniform_buffer, uint index, const void* value);
void uniformbuffer_upload(UniformBuffer* uniform_buffer, Renderer* renderer);
void uniformbuffer_bind_vs(UniformBuffer* uniform_buffer, Renderer* renderer);
void uniformbuffer_bind_ps(UniformBuffer* uniform_buffer, Renderer* renderer);