#pragma once
#include "api/astructs.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, const void* uniforms, uint uniforms_size);
AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, uint uniforms_size);
void auniformbuffer_delete(AUniformBuffer* uniform_buffer);

void auniformbuffer_bind_base(AUniformBuffer* uniform_buffer, ARenderer* renderer, uint index);
void auniformbuffer_set_data(AUniformBuffer* uniform_buffer, ARenderer* renderer, const void* uniforms, uint uniforms_size);