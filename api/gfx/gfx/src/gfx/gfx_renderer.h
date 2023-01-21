#pragma once
#include "ctx/ctx_types.h"
#include "gfx_types.h"

Renderer *renderer_create(Renderer *renderer, Context *context, int width, int height, LogCallbacks *log, int lhc);
void renderer_delete(Renderer *renderer);

void renderer_depth_stencil_set(Renderer *renderer, bool depth_enabled, bool stencil_enabled);
void renderer_rasterizer_set(Renderer *renderer, bool wireframe, bool cull_back, bool ccw);
void renderer_blend_set(Renderer *renderer, bool enabled);

mat4 renderer_perspective(Renderer *renderer, float fovy, float aspect, float zNear, float zFar);
mat4 renderer_ortho(Renderer *renderer, float left, float right, float bottom, float top, float near, float far);

float renderer_near(Renderer *renderer);
float renderer_far(Renderer *renderer);
