#include "pch.h"
#include "framebuffer.h"

Framebuffer* framebuffer_create(Framebuffer* framebuffer) {
	framebuffer->framebuffer = aframebuffer_create();
	return framebuffer;
}

void framebuffer_delete(Framebuffer* framebuffer) {
	aframebuffer_delete(framebuffer->framebuffer);
}

void framebuffer_attachment_color(Framebuffer* framebuffer, int width, int height) {
	aframebuffer_attachment_color(framebuffer->framebuffer, width, height);
}

void framebuffer_attachment_depth_stencil(Framebuffer* framebuffer, int width, int height) {
	aframebuffer_attachment_depth_stencil(framebuffer->framebuffer, width, height);
}

void framebuffer_attachment_bind_color(Framebuffer* framebuffer) {
	aframebuffer_attachment_bind_color(framebuffer->framebuffer);
}

void framebuffer_bind(Framebuffer* framebuffer) {
	aframebuffer_bind(framebuffer->framebuffer);
}

void framebuffer_unbind(Framebuffer* framebuffer) {
	aframebuffer_unbind(framebuffer->framebuffer);
}

bool framebuffer_check_status(Framebuffer* framebuffer) {
	return aframebuffer_check_status(framebuffer->framebuffer);
}