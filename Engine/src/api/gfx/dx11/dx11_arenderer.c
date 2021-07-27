#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/arenderer.h"

struct ARenderer {
	byte id;
};

ARenderer* arenderer_create() {
	ARenderer* renderer = m_malloc(sizeof(ARenderer));
	return renderer;
}

void arenderer_delete(ARenderer* renderer) {
	m_free(renderer, sizeof(ARenderer));
}

void arenderer_clear_buffers(ARenderer* renderer) {

}

void arenderer_clear_buffer_color(ARenderer* renderer) {

}

void arenderer_clear_buffer_depth(ARenderer* renderer) {

}

void arenderer_clear_color(ARenderer* renderer, float red, float green, float blue, float alpha) {

}

void arenderer_depth_test_set_enabled(ARenderer* renderer, bool enabled) {

}

void arenderer_cull_face_set_enabled(ARenderer* renderer, bool enabled) {

}

void arenderer_polygon_mode_fill(ARenderer* renderer) {

}

void arenderer_polygon_mode_line(ARenderer* renderer) {

}
#endif