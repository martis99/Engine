#pragma once
#include "gfx_types.h"

MeshRenderer *mesh_renderer_create(MeshRenderer *mesh_renderer, Renderer *renderer);
void mesh_renderer_delete(MeshRenderer *mesh_renderer);

void mesh_renderer_begin(MeshRenderer *mesh_renderer);
void mesh_renderer_render(MeshRenderer *mesh_renderer, int id, Transform *transform, MeshComponent *mesh_component);
void mesh_renderer_end(MeshRenderer *mesh_renderer);