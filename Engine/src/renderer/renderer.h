#pragma once
#include "structs.h"

Renderer* renderer_create(Renderer* renderer, Context* context, int width, int height);
void renderer_delete(Renderer* renderer);

void renderer_begin(Renderer* renderer);
void renderer_end(Renderer* renderer);

void renderer_clear_depth(Renderer* renderer);

void renderer_toggle_backface_culling(Renderer* renderer);
void renderer_toggle_fireframe(Renderer* renderer);

int renderer_get_mouse_entity(Renderer* renderer);