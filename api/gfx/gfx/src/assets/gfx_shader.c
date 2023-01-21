#include "gfx_shader.h"

#include "assets/image.h"
#include "gfx_texture.h"

Shader *shader_create(Shader *shader, Renderer *renderer, const char *vert, const char *frag, AShaderDesc desc)
{
	ashaderdesc_copy(&desc, &shader->desc);

	shader->shader = renderer->driver->shader_create(renderer->renderer, vert, frag, "Textures", desc.textures_count);
	if (shader->shader == NULL) {
		return NULL;
	}

	image_create(&shader->default_image, 1, 1, 4);
	uint data = (uint)0xffffffff;
	image_set_data(&shader->default_image, (unsigned char *)&data);
	if (texture_create(&shader->default_texture, renderer, &shader->default_image, A_REPEAT, A_LINEAR) == NULL) {
		return NULL;
	}

	return shader;
}

void shader_delete(Shader *shader, Renderer *renderer)
{
	ashaderdesc_delete(&shader->desc);
	image_delete(&shader->default_image);
	texture_delete(&shader->default_texture, renderer);
	renderer->driver->shader_delete(shader->shader, renderer->renderer);
}

void shader_bind(Shader *shader, Renderer *renderer)
{
	renderer->driver->shader_bind(shader->shader, renderer->renderer);
}
