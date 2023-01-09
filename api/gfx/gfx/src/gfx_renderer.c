#include "gfx_renderer.h"
#include "api/gfx/gfx_api_renderer.h"

Renderer* renderer_create(Renderer* renderer, Context* context, int width, int height, LogCallbacks* log) {
	renderer->renderer = arenderer_create(context->context, log);
	renderer->log = log;
	if (renderer->renderer == NULL) {
		return NULL;
	}
	renderer->width = width;
	renderer->height = height;
	renderer->draw_calls = 0;
	return renderer;
}

void renderer_delete(Renderer* renderer) {
	arenderer_delete(renderer->renderer);
}

void renderer_depth_stencil_set(Renderer* renderer, bool depth_enabled, bool stencil_enabled) {
	arenderer_depth_stencil_set(renderer->renderer, depth_enabled, stencil_enabled);
}

void renderer_rasterizer_set(Renderer* renderer, bool wireframe, bool cull_back, bool ccw) {
	renderer->cull_back = cull_back;
	arenderer_rasterizer_set(renderer->renderer, wireframe, cull_back, ccw);
}

void renderer_blend_set(Renderer* renderer, bool enabled) {
	arenderer_blend_set(renderer->renderer, enabled);
}

mat4 renderer_perspective(Renderer* renderer, float fovy, float aspect, float zNear, float zFar) {
	return arenderer_perspective(renderer->renderer, fovy, aspect, zNear, zFar);
}

mat4 renderer_ortho(Renderer* renderer, float left, float right, float bottom, float top, float near, float far) {
	return arenderer_ortho(renderer->renderer, left, right, bottom, top, near, far);
}

float renderer_near(Renderer* renderer) {
	return arenderer_near(renderer->renderer);
}

float renderer_far(Renderer* renderer) {
	return arenderer_far(renderer->renderer);
}