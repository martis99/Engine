#pragma once
#include "api/astructs.h"

AUniformBuffer* auniformbuffer_create();
void auniformbuffer_delete(AUniformBuffer* uniform_buffer);

void auniformbuffer_init_static(AUniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size);
void auniformbuffer_init_dynamic(AUniformBuffer* uniform_buffer, uint uniforms_size);
void auniformbuffer_bind_base(AUniformBuffer* uniform_buffer, uint index);
void auniformbuffer_set_data(AUniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size);