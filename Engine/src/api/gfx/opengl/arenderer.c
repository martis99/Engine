#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/arenderer.h"

#include "gl/gl_renderer.h"

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

void arenderer_clear(ARenderer* renderer, float red, float green, float blue, float alpha) {
	gl_clear(red, green, blue, alpha);
}
#endif