#include "pch.h"
#include "renderer.h"

#include "app.h"

Renderer* renderer_create(Renderer* renderer) {
	renderer->renderer = arenderer_create();
	return renderer;
}

void renderer_delete(Renderer* renderer) {
	arenderer_delete(renderer->renderer);
}

void renderer_begin(Renderer* renderer) {
	arenderer_clear(renderer->renderer, 0.1f, 0.1f, 0.1f, 1.0f);
}

void renderer_end(Renderer* renderer) {

}