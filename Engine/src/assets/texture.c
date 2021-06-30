#include "pch.h"
#include "texture.h"

Texture* texture_create(Texture* texture, AFilter filter) {
	texture->texture = atexture_create(filter);
	return texture;
}

Texture* texture_create_from_image(Texture* texture, Image* image, AFilter filter) {
	return texture_set_image(texture_create(texture, filter), image);
}

Texture* texture_set_image(Texture* texture, Image* image) {
	texture->width = image->width;
	texture->height = image->height;
	texture->channels = image->channels;

	atexture_set_data(texture->texture, image->width, image->height, image->channels, image->data);
	return texture;
}

void texture_bind(Texture* texture, int slot) {
	atexture_bind(texture->texture, slot);
}

void texture_delete(Texture* texture) {
	atexture_delete(texture->texture);
}