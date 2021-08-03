#pragma once
#include "structs.h"

Assets* assets_create(Assets* assets, Renderer* renderer);
void assets_delete(Assets* assets);

Shader* assets_shader_create(Assets* assets, const char* name, const char* vertex_source, const char* fragment_source, AValue* layout, uint layout_size, AValue* instance, uint instance_size, AValue* props, uint props_size, const char* textures, uint num_textures);
Shader* assets_shader_get(Assets* assets, const char* name);

Mesh* assets_mesh_create(Assets* assets, const char* name);
Mesh* assets_mesh_get(Assets* assets, const char* name);

Material* assets_material_create(Assets* assets, const char* name, Shader* shader);
Material* assets_material_get(Assets* assets, const char* name);

Image* assets_image_create(Assets* assets, const char* name, int width, int height, int channels);
Image* assets_image_load(Assets* assets, const char* name, const char* path);
Image* assets_image_get(Assets* assets, const char* name);

Texture* assets_texture_create_from_image(Assets* assets, const char* name, Image* image, AWrap wrap, AFilter filter);
Texture* assets_texture_get(Assets* assets, const char* name);

Font* assets_font_load(Assets* assets, const char* name, const char* path, int size);
Font* assets_font_get(Assets* assets, const char* name);

Model* assets_model_load(Assets* assets, const char* name, const char* path, const char* filename, Shader* shader, bool flipUVs, bool print);
Model* assets_model_get(Assets* assets, const char* name);