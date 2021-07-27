#pragma once
#include "structs.h"

Context* context_create(Context* context, Window* window);
void context_delete(Context* context);

void context_swap_buffers(Context* context);