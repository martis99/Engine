#include "pch.h"
#include "texture.h"

Texture* texture_create(Texture* texture, AWrap wrap, AFilter filter) {
	texture->texture = atexture_create(wrap, filter);
	return texture;
}

Texture* texture_create_from_image(Texture* texture, Renderer* renderer, Image* image, AWrap wrap, AFilter filter) {
	return texture_set_image(texture_create(texture, wrap, filter), renderer, image);
}

Texture* texture_set_image(Texture* texture, Renderer* renderer, Image* image) {
	texture->width = image->width;
	texture->height = image->height;
	texture->channels = image->channels;

	atexture_set_data(texture->texture, renderer->renderer, image->width, image->height, image->channels, image->data);
	return texture;
}

void texture_bind(Texture* texture, Renderer* renderer, int slot) {
	atexture_bind(texture->texture, renderer->renderer, slot);
}

void texture_delete(Texture* texture) {
	atexture_delete(texture->texture);
}