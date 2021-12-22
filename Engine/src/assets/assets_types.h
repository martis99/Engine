#pragma once
#include "eng_common_types.h"

typedef struct Image {
	int width;
	int height;
	int channels;
	bool from_file;
	unsigned char* data;
} Image;
