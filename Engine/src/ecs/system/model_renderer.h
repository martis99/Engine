#pragma once
#include "assets/assets.h"

typedef struct ModelRenderer {
	Shader* shader;
} ModelRenderer;

ModelRenderer* model_renderer_create(ModelRenderer* model_renderer, Assets* assets);
void model_renderer_delete(ModelRenderer* model_renderer);

void model_renderer_render(ModelRenderer* model_renderer, Ecs* ecs);