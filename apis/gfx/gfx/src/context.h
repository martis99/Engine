#pragma once
#include "gfx_types.h"

Context* context_create(Context* context, void* window);
void context_delete(Context* context);

void context_swap_buffers(Context* context);