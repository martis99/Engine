#include "pch.h"
#include "shader.h"

Shader* shader_create(Shader* shader, const char* src_vert, const char* src_frag) {
	shader->shader = ashader_create(src_vert, src_frag);
	ashader_bind_uniform_block(shader->shader, "Camera", 0);

	uniform_create(&shader->model, shader, "u_model", MAT4F, 1);
	uniform_create(&shader->entity, shader, "u_entity", VEC1I, 1);

	return shader;
}

void shader_delete(Shader* shader) {
	uniform_delete(&shader->model);
	uniform_delete(&shader->entity);
	ashader_delete(shader->shader);
}

void shader_bind(Shader* shader) {
	ashader_bind(shader->shader);
}

void shader_set_model(Shader* shader, mat4* model) {
	uniform_upload_mat4f(&shader->model, 1, model);
}

void shader_set_entity(Shader* shader, int entity) {
	uniform_upload_vec1i(&shader->entity, 1, &entity);
}

void shader_bind_uniform_block(Shader* shader, const char* name, uint index) {
	ashader_bind_uniform_block(shader->shader, name, index);
}