#include "pch.h"
#include "api/gfx/aframebuffer.h"
#include "gn_atypes.h"

AFramebuffer* aframebuffer_create(ARenderer* renderer, AAttachmentDesc* attachments, uint attachments_size, int width, int height) {
	AFramebuffer* framebuffer = m_malloc(sizeof(AFramebuffer));
	return framebuffer;
}

void aframebuffer_delete(AFramebuffer* framebuffer) {
	m_free(framebuffer, sizeof(AFramebuffer));
}

void aframebuffer_set_render_targets(AFramebuffer* framebuffer, ARenderer* renderer, uint* targets, uint targets_size) {

}

void aframebuffer_clear_attachment(AFramebuffer* framebuffer, ARenderer* renderer, uint id, const void* value) {

}

void aframebuffer_clear_depth_attachment(AFramebuffer* framebuffer, ARenderer* renderer, const void* value) {

}

void aframebuffer_read_pixel(AFramebuffer* framebuffer, ARenderer* renderer, uint id, int x, int y, void* pixel) {

}

void aframebuffer_draw(AFramebuffer* framebuffer, ARenderer* renderer, uint id) {

}