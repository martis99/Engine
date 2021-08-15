#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_attachment.h"
#include "gl/gl_defines.h"
#include "gl/gl_texture.h"
#include "gl/gl_buffer.h"

GLAttachment* gl_attachment_create(AAttachmentDesc desc, GLsizei width, GLsizei height, GLuint index) {
	GLAttachment* attachment = m_malloc(sizeof(GLAttachment));

	attachment->format = gl_atype_format(desc.type);
	attachment->type = gl_atype_type(desc.type);
	attachment->texture = gl_texture_create(gl_awrap(desc.wrap), gl_afilter(desc.filter), width, height, gl_atype_internal_format(desc.type), attachment->format, attachment->type, NULL, 1);
	if (attachment->texture == 0) {
		log_error("Failed to create texture");
		return NULL;
	}

	attachment->target = GL_COLOR_ATTACHMENT0 + index;
	if (gl_fb_attach_texture(attachment->target, attachment->texture) == A_FAIL) {
		log_error("Failed to attach texture to framebuffer");
		return NULL;
	}

	return attachment;
}

void gl_attachment_delete(GLAttachment* attachment) {
	if (attachment->texture != 0) {
		gl_texture_delete(attachment->texture);
		attachment->texture = 0;
	}
	m_free(attachment, sizeof(GLAttachment));
}

void gl_attachment_bind(GLAttachment* attachment, GLuint slot) {
	gl_texture_bind(attachment->texture, slot);
}

void gl_attachment_unbind(GLAttachment* attachment, GLuint slot) {
	gl_texture_bind(0, slot);
}

void gl_attachment_clear(GLAttachment* attachment, GLint index, const void* value) {
	gl_cb_clear(attachment->type, index, value);
}

void gl_attachment_read_pixel(GLAttachment* attachment, int x, int y, void* pixel) {
	gl_read_pixels(attachment->target, x, y, 1, 1, attachment->format, attachment->type, pixel);
}

#endif