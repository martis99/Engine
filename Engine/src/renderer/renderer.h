#pragma once
#include "api/gfx/arenderer.h"
#include "assets/framebuffer.h"

#include "assets/shader.h"
#include "assets/mesh.h"

typedef struct Renderer {
	ARenderer* renderer;
	Framebuffer framebuffer;
	Shader shader;
	Mesh mesh;
	bool backface_culling;
	bool wireframe;
} Renderer;

Renderer* renderer_create(Renderer* renderer, int width, int height);
void renderer_delete(Renderer* renderer);

void renderer_begin(Renderer* renderer);
void renderer_end(Renderer* renderer);

void renderer_clear_depth(Renderer* renderer);

void renderer_toggle_backface_culling(Renderer* renderer);
void renderer_toggle_fireframe(Renderer* renderer);