#include "pch.h"
#include "shader.h"

Shader* shader_create(Shader* shader, const char* src_vert, const char* src_frag) {
	shader->shader = ashader_create(src_vert, src_frag);

	uniform_create(&shader->view_projection, shader, "u_view_projection", MAT4F, 1);
	uniform_create(&shader->model, shader, "u_model", MAT4F, 1);

	return shader;
}

void shader_delete(Shader* shader) {
	uniform_delete(&shader->view_projection);
	uniform_delete(&shader->model);
	ashader_delete(shader->shader);
}

void shader_bind(Shader* shader, mat4* view_projection) {
	ashader_bind(shader->shader);
	uniform_upload_mat4f(&shader->view_projection, 1, view_projection);
}

void shader_set_model(Shader* shader, mat4* model) {
	uniform_upload_mat4f(&shader->model, 1, model);
}