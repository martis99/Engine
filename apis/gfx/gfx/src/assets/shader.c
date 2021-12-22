#include "shader.h"
#include "api/gfx/ashader.h"
#include "api/gfx/ashadergenerator.h"

#include "assets/image.h"
#include "texture.h"

Shader* shader_create(Shader* shader, Renderer* renderer, const char* vert, const char* frag, AShaderDesc desc) {
	ashaderdesc_copy(&desc, &shader->desc);

	size_t vs_size = strlen(vert) + 1 * sizeof(char) + 1600 * sizeof(char);
	size_t fs_size = strlen(frag) + 1 * sizeof(char) + 1600 * sizeof(char);
	char* src_vert = m_malloc(vs_size);
	char* src_frag = m_malloc(fs_size);

	ashadergenerator_generate(shader->desc, src_vert, src_frag, vert, frag);
	printf("%s\n", src_vert);
	printf("%s\n", src_frag);

	shader->shader = ashader_create(renderer->renderer, src_vert, src_frag, "Textures", desc.textures_count);
	if (shader->shader == NULL) {
		return NULL;
	}

	m_free(src_vert, vs_size);
	m_free(src_frag, fs_size);

	image_create(&shader->default_image, 1, 1, 4);
	uint data = (uint)0xffffffff;
	image_set_data(&shader->default_image, (unsigned char*)&data);
	if (texture_create(&shader->default_texture, renderer, &shader->default_image, A_REPEAT, A_LINEAR) == NULL) {
		return NULL;
	}

	return shader;
}

void shader_delete(Shader* shader) {
	ashaderdesc_delete(&shader->desc);
	image_delete(&shader->default_image);
	texture_delete(&shader->default_texture);
	ashader_delete(shader->shader);
}

void shader_bind(Shader* shader, Renderer* renderer) {
	ashader_bind(shader->shader, renderer->renderer);
}