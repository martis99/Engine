#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/aframebuffer.h"

#include "gl/gl_framebuffer.h"
#include "gl/gl_texture.h"

struct AFramebuffer {
	GLuint fb;
	GLuint attachment_color;
	GLuint attachment_depth;
};

AFramebuffer* aframebuffer_create() {
	AFramebuffer* framebuffer = m_malloc(sizeof(AFramebuffer));
	framebuffer->fb = gl_fb_create();
	framebuffer->attachment_color = 0;
	framebuffer->attachment_depth = 0;
	return framebuffer;
}

void aframebuffer_delete(AFramebuffer* framebuffer) {
	if (framebuffer->attachment_color != 0) {
		gl_texture_delete(framebuffer->attachment_color);
	}
	gl_fb_delete(framebuffer->fb);
	m_free(framebuffer, sizeof(AFramebuffer));
}

void aframebuffer_attachment_color(AFramebuffer* framebuffer, int width, int height) {
	framebuffer->attachment_color = gl_fb_attachment_color(framebuffer->fb, width, height);
}

void aframebuffer_attachment_depth_stencil(AFramebuffer* framebuffer, int width, int height) {
	framebuffer->attachment_depth = gl_fb_attachment_depth_stencil(framebuffer->fb, width, height);
}

void aframebuffer_attachment_bind_color(AFramebuffer* framebuffer) {
	gl_fb_attachment_bind_color(framebuffer->attachment_color);
}

void aframebuffer_bind(AFramebuffer* framebuffer) {
	gl_fb_bind(framebuffer->fb);
}

void aframebuffer_unbind(AFramebuffer* framebuffer) {
	gl_fb_bind(0);
}

bool aframebuffer_check_status(AFramebuffer* framebuffer) {
	return gl_fb_check_status(framebuffer->fb);
}
#endif