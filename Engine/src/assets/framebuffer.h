#pragma once
#include "types.h"

Framebuffer* framebuffer_create(Framebuffer* framebuffer, AAttachmentFormat* attachments, int attachments_size, int width, int height);
void framebuffer_delete(Framebuffer* framebuffer);

void framebuffer_bind(Framebuffer* framebuffer);
void framebuffer_unbind(Framebuffer* framebuffer);

bool framebuffer_check_status(Framebuffer* framebuffer);

void framebuffer_attachment_bind(Framebuffer* framebuffer, int index);

void framebuffer_color_attachments_draw(Framebuffer* framebuffer);
void framebuffer_color_attachment_clear_i(Framebuffer* framebuffer, int index, const int* value);
void framebuffer_color_attachment_clear_f(Framebuffer* framebuffer, int index, const float* value);
int framebuffer_color_attachment_read_pixel(Framebuffer* framebuffer, int index, int x, int y);