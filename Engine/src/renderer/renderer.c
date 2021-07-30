#include "pch.h"
#include "renderer.h"

#include "app.h"

#include "assets/shader.h"
#include "assets/mesh.h"

#include "input/mouse.h"

#include "assets/framebuffer.h"

Renderer* renderer_create(Renderer* renderer, Context* context, int width, int height) {
	renderer->renderer = arenderer_create(context->context);
	renderer->width = width;
	renderer->height = height;
	renderer->wireframe = 0;
	renderer->backface_culling = 1;
	return renderer;
}

void renderer_delete(Renderer* renderer) {
	arenderer_delete(renderer->renderer);
}

void renderer_begin(Renderer* renderer) {
	arenderer_clear_color(renderer->renderer, 0.1f, 0.1f, 0.1f, 1.0f);
	arenderer_clear_buffers(renderer->renderer);
	app.stats.draw_calls = 0;
}

void renderer_end(Renderer* renderer) {
	
}

void renderer_clear_depth(Renderer* renderer) {
	arenderer_clear_buffer_depth(renderer->renderer);
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

int renderer_get_mouse_entity(Renderer* renderer) {
	framebuffer_bind(&renderer->framebuffer);
	int x = (int)get_mouse_x();
	int y = renderer->height - (int)get_mouse_y();
	int pixel = framebuffer_color_attachment_read_pixel(&renderer->framebuffer, 1, x, y);
	framebuffer_unbind(&renderer->framebuffer);
	return pixel;
}