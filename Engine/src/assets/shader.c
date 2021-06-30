#include "pch.h"
#include "shader.h"

Shader* shader_create(Shader* shader, const char* src_vert, const char* src_frag) {
	shader->shader = ashader_create(src_vert, src_frag);
	return shader;
}

void shader_delete(Shader* shader) {
	ashader_delete(shader->shader);
}

void shader_bind(Shader* shader) {
	ashader_bind(shader->shader);
}