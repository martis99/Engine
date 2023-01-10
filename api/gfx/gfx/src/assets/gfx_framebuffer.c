#include "gfx_framebuffer.h"
#include "api/gfx/gfx_api_framebuffer.h"

Framebuffer *framebuffer_create(Framebuffer *framebuffer, Renderer *renderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height)
{
	framebuffer->framebuffer = aframebuffer_create(renderer->renderer, attachments, attachments_size, width, height);
	if (framebuffer->framebuffer == NULL) {
		return NULL;
	}
	return framebuffer;
}

void framebuffer_delete(Framebuffer *framebuffer, Renderer *renderer)
{
	aframebuffer_delete(framebuffer->framebuffer, renderer->renderer);
}

void framebuffer_set_render_targets(Framebuffer *framebuffer, Renderer *renderer, uint *targets, uint targets_size)
{
	aframebuffer_set_render_targets(framebuffer->framebuffer, renderer->renderer, targets, targets_size);
}

void framebuffer_clear_attachment(Framebuffer *framebuffer, Renderer *renderer, uint id, const void *value)
{
	aframebuffer_clear_attachment(framebuffer->framebuffer, renderer->renderer, id, value);
}

void framebuffer_clear_depth_attachment(Framebuffer *framebuffer, Renderer *renderer, const void *value)
{
	aframebuffer_clear_depth_attachment(framebuffer->framebuffer, renderer->renderer, value);
}

void framebuffer_read_pixel(Framebuffer *framebuffer, Renderer *renderer, uint id, int x, int y, void *pixel)
{
	aframebuffer_read_pixel(framebuffer->framebuffer, renderer->renderer, id, x, y, pixel);
}

void framebuffer_draw(Framebuffer *framebuffer, Renderer *renderer, uint id)
{
	aframebuffer_draw(framebuffer->framebuffer, renderer->renderer, id);
}
