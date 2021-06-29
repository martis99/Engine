#pragma once
#include "api/gfx/arenderer.h"

typedef struct Renderer {
	ARenderer* renderer;
} Renderer;

Renderer* renderer_create(Renderer* renderer);
void renderer_delete(Renderer* renderer);

void renderer_begin(Renderer* renderer);
void renderer_end(Renderer* renderer);