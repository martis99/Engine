#pragma once
#include "gfx_types.h"

ABuffer *buffer_create(ABuffer *buffer, AValue *values, uint values_size, const void *data);
void buffer_delete(ABuffer *buffer);

void buffer_set_value(ABuffer *buffer, uint index, const void *value);
