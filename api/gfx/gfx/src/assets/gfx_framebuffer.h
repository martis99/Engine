#pragma once
#include "gfx_types.h"

Framebuffer* framebuffer_create(Framebuffer* framebuffer, Renderer* renderer, AAttachmentDesc* attachments, uint attachments_size, int width, int height);
void framebuffer_delete(Framebuffer* framebuffer, Renderer* renderer);

void framebuffer_set_render_targets(Framebuffer* framebuffer, Renderer* renderer, uint* targets, uint targets_size);
void framebuffer_clear_attachment(Framebuffer* framebuffer, Renderer* renderer, uint id, const void* value);
void framebuffer_clear_depth_attachment(Framebuffer* framebuffer, Renderer* renderer, const void* value);
void framebuffer_read_pixel(Framebuffer* framebuffer, Renderer* renderer, uint id, int x, int y, void* pixel);
void framebuffer_draw(Framebuffer* framebuffer, Renderer* renderer, uint id);