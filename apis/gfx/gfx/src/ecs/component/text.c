#include "text.h"

Text text_create(const char* text, Font* font, vec4 color) {
	return (Text) { text, font, color };
}