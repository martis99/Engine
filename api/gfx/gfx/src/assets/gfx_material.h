#pragma once
#include "gfx_types.h"

Material* material_create(Material* material, Renderer* renderer, Shader* shader);
void material_delete(Material* material, Renderer* renderer);

void material_set_vs_value(Material* material, uint index, const void* value);
void material_set_ps_value(Material* material, uint index, const void* value);
void material_upload(Material* material, Renderer* renderer);
uint material_add_texture(Material* material, Texture* texture);

void material_bind(Material* material, Renderer* renderer);
