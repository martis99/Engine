#pragma once
#include "gfx_types.h"

LineRenderer* line_renderer_create(LineRenderer* line_renderer, Renderer* renderer, Transform transform);
void line_renderer_delete(LineRenderer* line_renderer);

void line_renderer_add(LineRenderer* line_renderer, vec3 start, vec3 end, vec4 color, int entity);
void line_renderer_render(LineRenderer* line_renderer);