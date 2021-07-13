#pragma once

typedef struct AFramebuffer AFramebuffer;

AFramebuffer* aframebuffer_create();
void aframebuffer_delete(AFramebuffer* framebuffer);

void aframebuffer_attachment_color(AFramebuffer* framebuffer, int width, int height);
void aframebuffer_attachment_depth_stencil(AFramebuffer* framebuffer, int width, int height);

void aframebuffer_attachment_bind_color(AFramebuffer* framebuffer);

void aframebuffer_bind(AFramebuffer* framebuffer);
void aframebuffer_unbind(AFramebuffer* framebuffer);

bool aframebuffer_check_status(AFramebuffer* framebuffer);