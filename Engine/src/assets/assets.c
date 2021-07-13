#include "pch.h"
#include "assets.h"

Assets* assets_create(Assets* assets) {
	assets->shaders = dic_create(20, sizeof(Shader));
	assets->meshes = dic_create(20, sizeof(Mesh));
	assets->materials = dic_create(20, sizeof(Material));
	assets->images = dic_create(20, sizeof(Image));
	assets->textures = dic_create(20, sizeof(Texture));
	assets->fonts = dic_create(5, sizeof(Font));
	return assets;
}

void assets_delete(Assets* assets) {
	dic_delete(assets->shaders, shader_delete);
	dic_delete(assets->meshes, mesh_delete);
	dic_delete(assets->materials, material_delete);
	dic_delete(assets->images, image_delete);
	dic_delete(assets->textures, texture_delete);
	dic_delete(assets->fonts, font_delete);
}

Shader* assets_shader_create(Assets* assets, const char* name, const char* vertex_source, const char* fragment_source) {
	return shader_create(dic_add(assets->shaders, name), vertex_source, fragment_source);
}

Shader* assets_shader_get(Assets* assets, const char* name) {
	return dic_get(assets->shaders, name);
}

Mesh* assets_mesh_create(Assets* assets, const char* name) {
	return mesh_create(dic_add(assets->meshes, name));
}

Mesh* assets_mesh_get(Assets* assets, const char* name) {
	return dic_get(assets->meshes, name);
}

Material* assets_material_create(Assets* assets, const char* name, Shader* shader) {
	return material_create(dic_add(assets->materials, name), shader);
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

Texture* assets_texture_create_from_image(Assets* assets, const char* name, Image* image, AWrap wrap, AFilter filter) {
	return texture_create_from_image(dic_add(assets->textures, name), image, wrap, filter);
}

Texture* assets_texture_get(Assets* assets, const char* name) {
	return dic_get(assets->textures, name);
}

Font* assets_font_load(Assets* assets, const char* name, const char* path, int size) {
	return font_load(dic_add(assets->fonts, name), assets, path, size);
}

Font* assets_font_get(Assets* assets, const char* name) {
	return dic_get(assets->fonts, name);
}