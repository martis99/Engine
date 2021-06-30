#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/atexture.h"

#include "gl/gl_texture.h"

struct ATexture {
	GLuint id;
};

ATexture* atexture_create(AFilter filter) {
	ATexture* texture = m_malloc(sizeof(ATexture));

	texture->id = gl_texture_create(W_REPEAT, filter);
	return texture;
}

ATexture* atexture_set_data(ATexture* texture, int width, int height, int channels, void* data) {
	gl_texture_image2d(texture->id, width, height, channels, data);
	return texture;
}

void atexture_bind(ATexture* texture, uint slot) {
	gl_texture_bind(texture->id, slot);
}

void atexture_delete(ATexture* texture) {
	gl_texture_delete(texture->id);
	m_free(texture, sizeof(ATexture));
}
#endif