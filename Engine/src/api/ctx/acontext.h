#pragma once
#include "api/astructs.h"

AContext* acontext_create(AWindow* window);
void acontext_delete(AContext* context);

void acontext_swap_buffers(AContext* context);