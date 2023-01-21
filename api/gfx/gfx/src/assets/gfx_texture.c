#include "gfx_texture.h"

Texture *texture_create(Texture *texture, Renderer *renderer, Image *image, AWrap wrap, AFilter filter)
{
	texture->width	  = image->width;
	texture->height	  = image->height;
	texture->channels = image->channels;

	texture->texture = renderer->driver->texture_create(renderer->renderer, wrap, filter, image->width, image->height, image->channels, image->data);
	if (texture->texture == NULL) {
		return NULL;
	}
	return texture;
}

void texture_bind(Texture *texture, Renderer *renderer, int slot)
{
	renderer->driver->texture_bind(texture->texture, renderer->renderer, slot);
}

void texture_delete(Texture *texture, Renderer *renderer)
{
	renderer->driver->texture_delete(texture->texture, renderer->renderer);
}
