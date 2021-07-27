#include "pch.h"
#include "uniform.h"

Uniform* uniform_create(Uniform* uniform, Shader* shader, const char* name, ADataType type, int count) {
	uniform->uniform = auniform_create(shader->shader, name, type, count);

	if (uniform->uniform == NULL) {
		log_error("Failed to create uniform");
		return NULL;
	}

	return uniform;
}

void uniform_delete(Uniform* uniform) {
	auniform_delete(uniform->uniform);
}

void uniform_set(Uniform* uniform, const void* data) {
	auniform_set(uniform->uniform, data);
}

void* uniform_get(Uniform* uniform) {
	return auniform_get(uniform->uniform);
}

size_t uniform_size(Uniform* uniform) {
	return auniform_size(uniform->uniform);
}

void uniform_upload(Uniform* uniform) {
	auniform_upload(uniform->uniform);
}

void uniform_upload_vec1i(Uniform* uniform, int count, const void* data) {
	auniform_upload_vec1i(uniform->uniform, count, data);
}

void uniform_upload_mat4f(Uniform* uniform, int count, void* data) {
	auniform_upload_mat4f(uniform->uniform, count, data);
}