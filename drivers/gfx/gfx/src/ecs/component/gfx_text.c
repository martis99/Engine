#include "gfx_text.h"

Text text_create(const char *text, EFont *font, vec4 color)
{
	return (Text){ text, font, color };
}
