#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_attachment.h"
#include "gl/gl_texture.h"

GLAttachment* gl_attachment_create(AAttachmentDesc desc, GLsizei width, GLsizei height, GLuint index) {
	GLAttachment* attachment = m_malloc(sizeof(GLAttachment));

	attachment->format = gl_atype_format(desc.type);
	attachment->type = gl_atype_type(desc.type);
	attachment->texture = gl_texture_create(gl_awrap(desc.wrap), gl_afilter(desc.filter), width, height, gl_atype_internal_format(desc.type), attachment->format, attachment->type, NULL);

	attachment->target = GL_COLOR_ATTACHMENT0 + index;
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment->target, GL_TEXTURE_2D, attachment->texture, 0);

	return attachment;
}

void gl_attachment_delete(GLAttachment* attachment) {
	gl_texture_delete(attachment->texture);
	m_free(attachment, sizeof(GLAttachment));
}

void gl_attachment_bind(GLAttachment* attachment, GLuint slot) {
	gl_texture_bind(attachment->texture, slot);
}

void gl_attachment_unbind(GLAttachment* attachment, GLuint slot) {
	gl_texture_bind(0, slot);
}

void gl_attachment_clear(GLAttachment* attachment, GLint index, const void* value) {
	switch (attachment->type) {
	case GL_UNSIGNED_INT: glClearBufferuiv(GL_COLOR, index, value);
	case GL_INT: glClearBufferiv(GL_COLOR, index, value);
	case GL_FLOAT: glClearBufferfv(GL_COLOR, index, value);
	}
}

void gl_attachment_read_pixel(GLAttachment* attachment, int x, int y, void* pixel) {
	glReadBuffer(attachment->target);
	glReadPixels(x, y, 1, 1, attachment->format, attachment->type, pixel);
}

#endif