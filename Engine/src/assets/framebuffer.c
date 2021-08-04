#include "pch.h"
#include "framebuffer.h"
#include "api/gfx/aframebuffer.h"

Framebuffer* framebuffer_create(Framebuffer* framebuffer, AAttachmentFormat* attachments, int attachments_size, int width, int height) {
	framebuffer->framebuffer = aframebuffer_create(attachments, attachments_size, width, height);
	return framebuffer;
}

void framebuffer_delete(Framebuffer* framebuffer) {
	aframebuffer_delete(framebuffer->framebuffer);
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

void framebuffer_attachment_bind(Framebuffer* framebuffer, int index) {
	aframebuffer_attachment_bind(framebuffer->framebuffer, index);
}

void framebuffer_color_attachments_draw(Framebuffer* framebuffer) {
	aframebuffer_color_attachments_draw(framebuffer->framebuffer);
}

void framebuffer_color_attachment_clear_i(Framebuffer* framebuffer, int index, const int* value) {
	aframebuffer_color_attachment_clear_i(framebuffer->framebuffer, index, value);
}

void framebuffer_color_attachment_clear_f(Framebuffer* framebuffer, int index, const float* value) {
	aframebuffer_color_attachment_clear_f(framebuffer->framebuffer, index, value);
}

int framebuffer_color_attachment_read_pixel(Framebuffer* framebuffer, int index, int x, int y) {
	return aframebuffer_color_attachment_read_pixel(framebuffer->framebuffer, index, x, y);
}