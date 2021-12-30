#pragma once
#include "gfx_api_types.h"

ARenderer* arenderer_create(AContext* context, LogCallbacks* log);
void arenderer_delete(ARenderer* renderer);

void arenderer_depth_stencil_set(ARenderer* renderer, bool depth_enabled, bool stencil_enabled);
void arenderer_rasterizer_set(ARenderer* renderer, bool wireframe, bool cull_back);
void arenderer_blend_set(ARenderer* renderer, bool enabled);