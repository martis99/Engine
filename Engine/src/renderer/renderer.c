#include "pch.h"
#include "renderer.h"

#include "app.h"

Renderer* renderer_create(Renderer* renderer) {
	renderer->renderer = arenderer_create();
	renderer->wireframe = 0;
	renderer->backface_culling = 1;
	return renderer;
}

void renderer_delete(Renderer* renderer) {
	arenderer_delete(renderer->renderer);
}

void renderer_begin(Renderer* renderer) {
	arenderer_clear(renderer->renderer, 0.1f, 0.1f, 0.1f, 1.0f);
	app.stats.draw_calls = 0;
}

void renderer_end(Renderer* renderer) {

}

void renderer_clear_depth(Renderer* renderer) {
	arenderer_clear_depth(renderer->renderer);
}

void renderer_toggle_backface_culling(Renderer* renderer) {
	renderer->backface_culling = 1 - renderer->backface_culling;
	arenderer_cull_face_set_enabled(renderer->renderer, renderer->backface_culling);
}

void renderer_toggle_fireframe(Renderer* renderer) {
	renderer->wireframe = 1 - renderer->wireframe;
	if (renderer->wireframe == 0) {
		arenderer_polygon_mode_fill(renderer->renderer);
	} else {
		arenderer_polygon_mode_line(renderer->renderer);
	}
}