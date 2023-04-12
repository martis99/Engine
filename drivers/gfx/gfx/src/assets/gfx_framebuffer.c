#include "gfx_framebuffer.h"

Framebuffer *framebuffer_create(Framebuffer *framebuffer, Renderer *renderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height)
{
	renderer->driver->fb_create(framebuffer->framebuffer, renderer->renderer, attachments, attachments_size, width, height);
	return framebuffer;
}

void framebuffer_delete(Framebuffer *framebuffer, Renderer *renderer)
{
	renderer->driver->fb_delete(framebuffer->framebuffer, renderer->renderer);
}

void framebuffer_bind_render_targets(Framebuffer *framebuffer, Renderer *renderer, uint *targets, uint targets_size)
{
	renderer->driver->fb_bind_render_targets(framebuffer->framebuffer, renderer->renderer, targets, targets_size);
}

void framebuffer_unbind_render_targets(Framebuffer *framebuffer, Renderer *renderer, uint *targets, uint targets_size)
{
	renderer->driver->fb_unbind_render_targets(framebuffer->framebuffer, renderer->renderer, targets, targets_size);
}

void framebuffer_clear_attachment(Framebuffer *framebuffer, Renderer *renderer, uint id, const void *value)
{
	renderer->driver->fb_clear_attachment(framebuffer->framebuffer, renderer->renderer, id, value);
}

void framebuffer_clear_depth_attachment(Framebuffer *framebuffer, Renderer *renderer, const void *value)
{
	renderer->driver->fb_clear_depth_attachment(framebuffer->framebuffer, renderer->renderer, value);
}

void framebuffer_read_pixel(Framebuffer *framebuffer, Renderer *renderer, uint id, int x, int y, void *pixel)
{
	renderer->driver->fb_read_pixel(framebuffer->framebuffer, renderer->renderer, id, x, y, pixel);
}

void framebuffer_draw(Framebuffer *framebuffer, Renderer *renderer, uint id)
{
	renderer->driver->fb_draw(framebuffer->framebuffer, renderer->renderer, id);
}
