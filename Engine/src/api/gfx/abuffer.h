#pragma once
#include "api/atypes.h"

ABuffer* abuffer_create(AValue* values, uint values_size, AShader* shader);
void abuffer_delete(ABuffer* buffer);

void abuffer_set_value(ABuffer* buffer, uint index, const void* value);