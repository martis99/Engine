#pragma once
#include "api/gfx/aframebuffer.h"

typedef struct Framebuffer {
	AFramebuffer* framebuffer;
} Framebuffer;

Framebuffer* framebuffer_create(Framebuffer* framebuffer);
void framebuffer_delete(Framebuffer* framebuffer);

void framebuffer_attachment_color(Framebuffer* framebuffer, int width, int height);
void framebuffer_attachment_depth_stencil(Framebuffer* framebuffer, int width, int height);

void framebuffer_attachment_bind_color(Framebuffer* framebuffer);

void framebuffer_bind(Framebuffer* framebuffer);
void framebuffer_unbind(Framebuffer* framebuffer);

bool framebuffer_check_status(Framebuffer* framebuffer);