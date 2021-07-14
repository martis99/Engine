#pragma once
#include "api/gfx/auniform_buffer.h"

typedef struct UniformBuffer {
	AUniformBuffer* uniform_buffer;
} UniformBuffer;

UniformBuffer* uniformbuffer_create(UniformBuffer* uniform_buffer);
void uniformbuffer_delete(UniformBuffer* uniform_buffer);

void uniformbuffer_init_static(UniformBuffer* uniform_buffer, const void* data, uint size);
void uniformbuffer_init_dynamic(UniformBuffer* uniform_buffer, uint size);
void uniformbuffer_bind_base(UniformBuffer* uniform_buffer, uint index);
void uniformbuffer_set_data(UniformBuffer* uniform_buffer, const void* data, uint size);