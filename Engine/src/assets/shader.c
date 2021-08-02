#include "pch.h"
#include "shader.h"

#include "image.h"
#include "texture.h"

Shader* shader_create(Shader* shader, Renderer* renderer, const char* src_vert, const char* src_frag, AValue* layout, uint layout_size, AValue* props, uint props_size, const char* textures, uint num_textures) {
	shader->shader = ashader_create(renderer->renderer, src_vert, src_frag, textures, num_textures);
	ashader_bind_uniform_block(shader->shader, "Camera", 0);

	shader->layout = m_malloc(layout_size);
	memcpy(shader->layout, layout, layout_size);
	shader->layout_size = layout_size;

	shader->props = m_malloc(props_size);
	memcpy(shader->props, props, props_size);
	shader->props_size = props_size;

	image_create(&shader->default_image, 1, 1, 4);
	uint data = (uint)0xffffffff;
	image_set_data(&shader->default_image, (unsigned char*)&data);
	texture_create(&shader->default_texture, renderer, &shader->default_image, A_REPEAT, A_LINEAR);

	shader->num_textures = num_textures;

	return shader;
}

void shader_delete(Shader* shader) {
	image_delete(&shader->default_image);
	texture_delete(&shader->default_texture);
	m_free(shader->layout, shader->layout_size);
	m_free(shader->props, shader->props_size);
	ashader_delete(shader->shader);
}

void shader_bind(Shader* shader, Renderer* renderer) {
	ashader_bind(shader->shader, renderer->renderer);
}

void shader_bind_uniform_block(Shader* shader, const char* name, uint index) {
	ashader_bind_uniform_block(shader->shader, name, index);
}