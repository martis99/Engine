#pragma once
#include "gfx_api_types.h"

#include "math/math_types.h"

ARenderer *arenderer_create(AContext *context, LogCallbacks *log, int lhc);
void arenderer_delete(ARenderer *renderer);

void arenderer_depth_stencil_set(ARenderer *renderer, bool depth_enabled, bool stencil_enabled);
void arenderer_rasterizer_set(ARenderer *renderer, bool wireframe, bool cull_back, bool ccw);
void arenderer_blend_set(ARenderer *renderer, bool enabled);

mat4 arenderer_perspective(ARenderer *renderer, float fovy, float aspect, float zNear, float zFar);
mat4 arenderer_ortho(ARenderer *renderer, float left, float right, float bottom, float top, float near, float far);

float arenderer_near(ARenderer *renderer);
float arenderer_far(ARenderer *renderer);
