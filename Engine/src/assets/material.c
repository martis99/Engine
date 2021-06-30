#include "pch.h"
#include "material.h"
#include "uniform.h"

Material* material_create(Material* material, Shader* shader) {
	material->shader = shader;
	material->uniforms = dic_create(20, sizeof(Uniform));

	return material;
}

void material_delete(Material* material) {
	dic_delete(material->uniforms, uniform_delete);
}

void material_bind(Material* material) {
	dic_exec(material->uniforms, uniform_upload);
}

static void set_uniform(Material* material, const char* name, AUniformType type, int count, const void* data) {
	Uniform* uniform = uniform_create(dic_add(material->uniforms, name), material->shader, name, type, count);
	uniform_set(uniform, data);
}

void material_set_vec4f(Material* material, const char* name, int count, vec4* value) {
	set_uniform(material, name, VEC4F, count, value);
}