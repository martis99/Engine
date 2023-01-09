#include "api/gfx/gfx_api_renderer.h"

#include "gfx_none_types.h"

#include <math/maths.h>

ARenderer* arenderer_create(AContext* context, LogCallbacks* log) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_depth_stencil_set(ARenderer* renderer, bool depth_enabled, bool stencil_enabled) {

}

void arenderer_rasterizer_set(ARenderer* renderer, bool wireframe, bool cull_back, bool ccw) {

}

void arenderer_blend_set(ARenderer* renderer, bool enabled) {

}

mat4 arenderer_perspective(ARenderer* renderer, float fovy, float aspect, float zNear, float zFar) {
	return mat4_perspective1(fovy, aspect, zNear, zFar);
}

mat4 arenderer_ortho(ARenderer* renderer, float left, float right, float bottom, float top, float znear, float zfar) {
	return mat4_ortho1(left, right, bottom, top, znear, zfar);
}

float arenderer_near(ARenderer* renderer) {
	return -1;
}

float arenderer_far(ARenderer* renderer) {
	return 1;
}