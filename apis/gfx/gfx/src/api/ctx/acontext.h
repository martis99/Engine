#pragma once
#include "api/gfx/gfx_api_types.h"

AContext* acontext_create(void* window, LogCallbacks* log);
void acontext_delete(AContext* context);

void acontext_swap_buffers(AContext* context);