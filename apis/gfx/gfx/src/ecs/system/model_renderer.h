#pragma once
#include "gfx_types.h"

ModelRenderer* model_renderer_create(ModelRenderer* model_renderer, Renderer* renderer);
void model_renderer_delete(ModelRenderer* model_renderer);

void model_renderer_render(ModelRenderer* model_renderer, Ecs* ecs);