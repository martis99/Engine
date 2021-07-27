#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/aframebuffer.h"
#include "gl_astructs.h"

#include "gl/gl_framebuffer.h"
#include "gl/gl_texture.h"

AFramebuffer* aframebuffer_create(AAttachmentFormat* attachments, int attachments_size, int width, int height) {
	int attachments_count = attachments_size / sizeof(AAttachmentFormat);

	AFramebuffer* framebuffer = m_malloc(sizeof(AFramebuffer));
	framebuffer->fb = gl_fb_create();
	framebuffer->attachments = m_malloc(sizeof(GLuint) * attachments_count);
	framebuffer->attachments_count = attachments_count;

	for (int i = 0; i < attachments_count; i++) {
		switch (attachments[i]) {
		case A_RGBA8: framebuffer->attachments[i] = gl_fb_color_attachment(framebuffer->fb, GL_RGBA8, GL_RGBA, width, height, i); break;
		case A_RED_INTEGER: framebuffer->attachments[i] = gl_fb_color_attachment(framebuffer->fb, GL_R32I, GL_RED_INTEGER, width, height, i); break;
		case A_DEPTH24STENCIL8: framebuffer->attachments[i] = gl_fb_depth_stencil_attachment(framebuffer->fb, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, width, height); break;
		default: break;
		}
	}

	return framebuffer;
}

void aframebuffer_delete(AFramebuffer* framebuffer) {
	for (int i = 0; i < framebuffer->attachments_count; i++) {
		gl_texture_delete(framebuffer->attachments[i]);
	}
	m_free(framebuffer->attachments, sizeof(GLuint) * framebuffer->attachments_count);
	gl_fb_delete(framebuffer->fb);
	m_free(framebuffer, sizeof(AFramebuffer));
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

void aframebuffer_attachment_bind(AFramebuffer* framebuffer, int index) {
	gl_fb_attachment_bind(framebuffer->attachments[index]);
}

void aframebuffer_color_attachments_draw(AFramebuffer* framebuffer) {
	GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT0 + 1 };
	glDrawBuffers(2, buffers);
}

void aframebuffer_color_attachment_clear_i(AFramebuffer* framebuffer, int index, const int* value) {
	gl_fb_color_attachment_clear_i(index, value);
}

void aframebuffer_color_attachment_clear_f(AFramebuffer* framebuffer, int index, const float* value) {
	gl_fb_color_attachment_clear_f(index, value);
}

int aframebuffer_color_attachment_read_pixel(AFramebuffer* framebuffer, int index, int x, int y) {
	return gl_fb_color_attachment_read_pixel(index, x, y);
}
#endif