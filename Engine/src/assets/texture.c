#include "pch.h"
#include "texture.h"

Texture* texture_create(Texture* texture, Renderer* renderer, Image* image, AWrap wrap, AFilter filter) {
	texture->width = image->width;
	texture->height = image->height;
	texture->channels = image->channels;

	texture->texture = atexture_create(renderer->renderer, wrap, filter, image->width, image->height, image->channels, image->data);
	return texture;
}

void texture_bind(Texture* texture, Renderer* renderer, int slot) {
	atexture_bind(texture->texture, renderer->renderer, slot);
}

void texture_delete(Texture* texture) {
	atexture_delete(texture->texture);
}