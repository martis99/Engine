#include "gfx_gl_attachment.h"
#include "gl/gl_defines.h"
#include "gl/gl_texture.h"
#include "gl/gl_buffer.h"

GLAttachment* gl_attachment_create(ARenderer* renderer, AAttachmentDesc desc, GLsizei width, GLsizei height, GLuint index) {
	GLAttachment* attachment = m_malloc(sizeof(GLAttachment));

	attachment->format = gl_atype_format(desc.type);
	attachment->type = gl_atype_type(desc.type);
	attachment->texture = gl_texture_create(renderer->error, gl_awrap(desc.wrap), desc.filter, width, height, gl_atype_internal_format(desc.type), attachment->format, attachment->type, NULL);
	if (attachment->texture == 0) {
		log_msg(renderer->log, "Failed to create texture");
		return NULL;
	}

	attachment->target = GL_COLOR_ATTACHMENT0 + index;
	if (gl_fb_attach_texture(renderer->error, attachment->target, attachment->texture) == A_FAIL) {
		log_msg(renderer->log, "Failed to attach texture to framebuffer");
		return NULL;
	}

	return attachment;
}

void gl_attachment_delete(GLAttachment* attachment, ARenderer* renderer) {
	if (attachment->texture != 0) {
		gl_texture_delete(renderer->error, attachment->texture);
		attachment->texture = 0;
	}
	m_free(attachment, sizeof(GLAttachment));
}

void gl_attachment_bind(ARenderer* renderer, GLAttachment* attachment, GLuint slot) {
	gl_texture_bind(renderer->error, attachment->texture, slot);
}

void gl_attachment_unbind(ARenderer* renderer, GLAttachment* attachment, GLuint slot) {
	gl_texture_bind(renderer->error, 0, slot);
}

void gl_attachment_clear(ARenderer* renderer, GLAttachment* attachment, GLint index, const void* value) {
	gl_cb_clear(renderer->error, attachment->type, index, value);
}

void gl_attachment_read_pixel(ARenderer* renderer, GLAttachment* attachment, int x, int y, void* pixel) {
	gl_read_pixels(renderer->error, attachment->target, x, y, 1, 1, attachment->format, attachment->type, pixel);
}