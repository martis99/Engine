#pragma once
#include "gfx_gl_types.h"

GLAttachment* gl_attachment_create(ARenderer* renderer, AAttachmentDesc desc, GLsizei width, GLsizei height, GLuint index);
void gl_attachment_delete(GLAttachment* attachment, ARenderer* renderer);

void gl_attachment_bind(ARenderer* renderer, GLAttachment* attachment, GLuint slot);
void gl_attachment_unbind(ARenderer* renderer, GLAttachment* attachment, GLuint slot);
void gl_attachment_clear(ARenderer* renderer, GLAttachment* attachment, GLint index, const void* value);
void gl_attachment_read_pixel(ARenderer* renderer, GLAttachment* attachment, int x, int y, void* pixel);