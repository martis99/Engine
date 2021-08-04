#pragma once
#include "types.h"

Image* image_create(Image* image, int width, int height, int channels);
void image_delete(Image* image);

void image_set_data(Image* image, unsigned char* data);
void image_set_color(Image* image, int color);
void image_set_pixels(Image* image, int x, int y, int width, int height, unsigned char* pixels);

void image_reverse_y(Image* image);

Image* image_load(Image* image, const char* path);
void image_save(Image* image, const char* path);