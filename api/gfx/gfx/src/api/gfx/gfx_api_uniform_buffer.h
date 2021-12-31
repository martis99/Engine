#pragma once
#include "gfx_api_types.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, uint slot, uint data_size, const void* data);
AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, uint slot, uint data_size);
void auniformbuffer_delete(AUniformBuffer* uniform_buffer, ARenderer* renderer);

void auniformbuffer_upload(AUniformBuffer* uniform_buffer, ARenderer* renderer, const void* data, uint data_size);
void auniformbuffer_bind_vs(AUniformBuffer* uniform_buffer, ARenderer* renderer);
void auniformbuffer_bind_ps(AUniformBuffer* uniform_buffer, ARenderer* renderer);