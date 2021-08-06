#pragma once
#include "types.h"

Model* model_create(Model* model);
void model_delete(Model* model);

void model_draw(Model* model, Renderer* renderer, Shader* shader, mat4 transformation, int entity);

Model* model_load(Model* model, Renderer* renderer, const char* path, const char* filename, Shader* shader, bool flipUVs, bool print);