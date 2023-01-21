#include "gfx_renderer.h"

#include "gfx_shader_creator.h"

#include "gfx_driver.h"

Renderer *renderer_create(Renderer *renderer, Context *context, int width, int height, LogCallbacks *log, int lhc)
{
	renderer->driver = gfx_driver_get(context->driver_str);
	if (renderer->driver == NULL) {
		log_error("GFX Driver not found");
		return NULL;
	}

	renderer->renderer = renderer->driver->renderer_create(context->context, log, lhc);
	renderer->log	   = log;
	renderer->lhc	   = lhc;

	if (renderer->renderer == NULL) {
		return NULL;
	}
	renderer->width	     = width;
	renderer->height     = height;
	renderer->draw_calls = 0;

	if (gfx_sc_create(&renderer->shader_creator, renderer) == NULL) {
		log_error("Failed to create shader creator");
		return NULL;
	}

	return renderer;
}

void renderer_delete(Renderer *renderer)
{
	gfx_sc_delete(&renderer->shader_creator);
	renderer->driver->renderer_delete(renderer->renderer);
}

void renderer_depth_stencil_set(Renderer *renderer, bool depth_enabled, bool stencil_enabled)
{
	renderer->driver->renderer_depth_stencil_set(renderer->renderer, depth_enabled, stencil_enabled);
}

void renderer_rasterizer_set(Renderer *renderer, bool wireframe, bool cull_back, bool ccw)
{
	renderer->cull_back = cull_back;
	renderer->driver->renderer_rasterizer_set(renderer->renderer, wireframe, cull_back, ccw);
}

void renderer_blend_set(Renderer *renderer, bool enabled)
{
	renderer->driver->renderer_blend_set(renderer->renderer, enabled);
}

mat4 renderer_perspective(Renderer *renderer, float fovy, float aspect, float zNear, float zFar)
{
	return renderer->driver->renderer_perspective(renderer->renderer, fovy, aspect, zNear, zFar);
}

mat4 renderer_ortho(Renderer *renderer, float left, float right, float bottom, float top, float near, float far)
{
	return renderer->driver->renderer_ortho(renderer->renderer, left, right, bottom, top, near, far);
}

float renderer_near(Renderer *renderer)
{
	return renderer->driver->renderer_near(renderer->renderer);
}

float renderer_far(Renderer *renderer)
{
	return renderer->driver->renderer_far(renderer->renderer);
}
