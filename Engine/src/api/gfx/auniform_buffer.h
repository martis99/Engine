#pragma once
#include "api/astructs.h"

AUniformBuffer* auniformbuffer_create_static(ARenderer* renderer, AValue* values, uint values_size, const void* data);
AUniformBuffer* auniformbuffer_create_dynamic(ARenderer* renderer, AValue* values, uint values_size);
void auniformbuffer_delete(AUniformBuffer* uniform_buffer);

void auniformbuffer_set_value(AUniformBuffer* uniform_buffer, uint index, const void* value);
void auniformbuffer_upload(AUniformBuffer* uniform_buffer, ARenderer* renderer);
void auniformbuffer_bind(AUniformBuffer* uniform_buffer, ARenderer* renderer, uint slot);