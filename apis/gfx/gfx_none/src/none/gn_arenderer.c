#include "api/gfx/arenderer.h"
#include "gn_atypes.h"

ARenderer* arenderer_create(AContext* context, LogCallbacks* log) {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_depth_stencil_set(ARenderer* renderer, bool depth_enabled, bool stencil_enabled) {

}

void arenderer_rasterizer_set(ARenderer* renderer, bool wireframe, bool cull_back) {

}

void arenderer_blend_set(ARenderer* renderer, bool enabled) {

}