#pragma once
#ifdef GAPI_OPENGL
#include "gl_atypes.h"

GLAttachment* gl_attachment_create(AAttachmentDesc desc, GLsizei width, GLsizei height, GLuint index);
void gl_attachment_delete(GLAttachment* attachment);

void gl_attachment_bind(GLAttachment* attachment, GLuint slot);
void gl_attachment_unbind(GLAttachment* attachment, GLuint slot);
void gl_attachment_clear(GLAttachment* attachment, GLint index, const void* value);
void gl_attachment_read_pixel(GLAttachment* attachment, int x, int y, void* pixel);

#endif