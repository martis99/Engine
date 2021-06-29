#pragma once

typedef struct AContext AContext;
typedef struct AWindow AWindow;

AContext* acontext_create(AWindow* window);
void acontext_delete(AContext* context);

void acontext_swap_buffers(AContext* context);