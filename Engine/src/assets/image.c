#include "image.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include "log.h"
#include "mem.h"

#include <memory.h>
#include <math.h>

Image *image_create(Image *image, int width, int height, int channels)
{
	image->width	 = width;
	image->height	 = height;
	image->channels	 = channels;
	image->data	 = (unsigned char *)m_malloc(sizeof(unsigned char) * width * height * channels);
	image->from_file = 0;
	return image;
}

void image_delete(Image *image)
{
	if (image->from_file == 0) {
		m_free(image->data, sizeof(unsigned char) * image->width * image->height * image->channels);
	} else {
		stbi_image_free(image->data);
	}
}

void image_set_data(Image *image, unsigned char *data)
{
	memcpy(image->data, data, sizeof(unsigned char) * image->width * image->height * image->channels);
}

void image_set_color(Image *image, int color)
{
	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			int id		= y * image->width + x;
			image->data[id] = color;
		}
	}
}

#define MIN(a, b) (a) < (b) ? (a) : (b)

void image_set_pixels(Image *image, int x, int y, int width, int height, unsigned char *pixels)
{
	for (int py = 0; py < height; py++) {
		for (int px = 0; px < width; px++) {
			int dx = x + px;
			int dy = y + py;

			int p_id = py * width + px;
			int d_id = dy * image->width + dx;

			image->data[d_id] = MIN(image->data[d_id] + pixels[p_id], 255);
		}
	}
}

void image_reverse_y(Image *image)
{
	int c = image->height / 2;
	for (int y = 0; y < c; y++) {
		for (int x = 0; x < image->width; x++) {
			int t_id = y * image->width + x;
			int b_id = (image->height - y - 1) * image->width + x;

			unsigned char t	  = image->data[t_id];
			image->data[t_id] = image->data[b_id];
			image->data[b_id] = t;
		}
	}
}

Image *image_load(Image *image, const char *path)
{
	stbi_set_flip_vertically_on_load(1);
	image->data	= stbi_load(path, &image->width, &image->height, &image->channels, 4);
	image->channels = 4;
	if (image->data == 0) {
		log_error("Failed to load image");
		return NULL;
	}

	image->from_file = 1;
	return image;
}

void image_save(Image *image, const char *path)
{
	stbi_write_png(path, image->width, image->height, image->channels, image->data, image->width * image->channels);
}
