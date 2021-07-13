#pragma once
#include "api/gfx/aatachmentformat.h"

typedef struct AFramebuffer AFramebuffer;

AFramebuffer* aframebuffer_create(AAttachmentFormat* attachments, int attachments_size, int width, int height);
void aframebuffer_delete(AFramebuffer* framebuffer);

void aframebuffer_bind(AFramebuffer* framebuffer);
void aframebuffer_unbind(AFramebuffer* framebuffer);

bool aframebuffer_check_status(AFramebuffer* framebuffer);

void aframebuffer_attachment_bind(AFramebuffer* framebuffer, int index);

void aframebuffer_color_attachments_draw(AFramebuffer* framebuffer);
void aframebuffer_color_attachment_clear_i(AFramebuffer* framebuffer, int index, const int* value);
void aframebuffer_color_attachment_clear_f(AFramebuffer* framebuffer, int index, const float* value);
int aframebuffer_color_attachment_read_pixel(AFramebuffer* framebuffer, int index, int x, int y);