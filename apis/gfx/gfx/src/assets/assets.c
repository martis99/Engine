#include "assets.h"

#include "shader.h"
#include "mesh.h"
#include "material.h"
#include "assets/image.h"
#include "texture.h"
#include "font.h"
#include "uniform_buffer.h"
#include "model.h"

#include "utils/dictionary.h"

Assets* assets_create(Assets* assets, Renderer* renderer) {
	assets->renderer = renderer;
	assets->shaders = dic_create(20, sizeof(Shader));
	assets->meshes = dic_create(20, sizeof(Mesh));
	assets->materials = dic_create(20, sizeof(Material));
	assets->images = dic_create(20, sizeof(Image));
	assets->textures = dic_create(20, sizeof(Texture));
	assets->fonts = dic_create(5, sizeof(Font));
	assets->models = dic_create(10, sizeof(Model));
	return assets;
}

void assets_delete(Assets* assets) {
	dic_delete_arg(assets->shaders, shader_delete, assets->renderer);
	dic_delete_arg(assets->meshes, mesh_delete, assets->renderer);
	dic_delete_arg(assets->materials, material_delete, assets->renderer);
	dic_delete(assets->images, image_delete);
	dic_delete_arg(assets->textures, texture_delete, assets->renderer);
	dic_delete_arg(assets->fonts, font_delete, assets->renderer);
	dic_delete_arg(assets->models, model_delete, assets->renderer);
}

Shader* assets_shader_create(Assets* assets, const char* name, const char* vertex_source, const char* fragment_source, AShaderDesc desc) {
	return shader_create(dic_add(assets->shaders, name), assets->renderer, vertex_source, fragment_source, desc);
}

Shader* assets_shader_get(Assets* assets, const char* name) {
	return dic_get(assets->shaders, name);
}

Mesh* assets_mesh_create(Assets* assets, const char* name, Shader* shader, AMeshData data, APrimitive primitive) {
	return mesh_create(dic_add(assets->meshes, name), assets->renderer, shader, data, primitive);
}

Mesh* assets_mesh_create_cube(Assets* assets, const char* name, Shader* shader) {
	return mesh_create_cube(dic_add(assets->meshes, name), assets->renderer, shader);
}

Mesh* assets_mesh_get(Assets* assets, const char* name) {
	return dic_get(assets->meshes, name);
}

Material* assets_material_create(Assets* assets, const char* name, Shader* shader) {
	return material_create(dic_add(assets->materials, name), assets->renderer, shader);
}

Material* assets_material_get(Assets* assets, const char* name) {
	return dic_get(assets->materials, name);
}

Image* assets_image_create(Assets* assets, const char* name, int width, int height, int channels) {
	return image_create(dic_add(assets->images, name), width, height, channels);
}

Image* assets_image_load(Assets* assets, const char* name, const char* path) {
	return image_load(dic_add(assets->images, name), path);
}

Image* assets_image_get(Assets* assets, const char* name) {
	return dic_get(assets->images, name);
}

Texture* assets_texture_create(Assets* assets, const char* name, Image* image, AWrap wrap, AFilter filter) {
	return texture_create(dic_add(assets->textures, name), assets->renderer, image, wrap, filter);
}

Texture* assets_texture_get(Assets* assets, const char* name) {
	return dic_get(assets->textures, name);
}

Font* assets_font_load(Assets* assets, const char* name, const char* path, int size) {
	return font_load(dic_add(assets->fonts, name), assets->renderer, path, size);
}

Font* assets_font_get(Assets* assets, const char* name) {
	return dic_get(assets->fonts, name);
}

Model* assets_model_load(Assets* assets, const char* name, const char* path, const char* filename, Shader* shader, bool flipUVs, bool print) {
	return model_load(dic_add(assets->models, name), assets->renderer, path, filename, shader, flipUVs, print);
}

Model* assets_model_get(Assets* assets, const char* name) {
	return dic_get(assets->models, name);
}