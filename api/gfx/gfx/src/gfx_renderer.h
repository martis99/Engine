#pragma once
#include "gfx_types.h"

Renderer* renderer_create(Renderer* renderer, Context* context, int width, int height, LogCallbacks* log);
void renderer_delete(Renderer* renderer);

void renderer_depth_stencil_set(Renderer* renderer, bool depth_enabled, bool stencil_enabled);
void renderer_rasterizer_set(Renderer* renderer, bool wireframe, bool cull_back, bool ccw);
void renderer_blend_set(Renderer* renderer, bool enabled);