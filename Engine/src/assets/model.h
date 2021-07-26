#pragma once
#include "mesh.h"
#include "material.h"

typedef struct ModelMesh {
	Mesh* mesh;
	uint material;
} ModelMesh;

typedef struct ModelNode ModelNode;

struct ModelNode {
	Array meshes;
	Array nodes;
	mat4 transformation;
};

typedef struct Model {
	ModelNode node;
	Array materials;
	Array images;
	Array textures;
} Model;

Model* model_create(Model* model);
void model_delete(Model* model);

void model_draw(Model* model, Shader* shader, mat4 transformation);

Model* model_load(Model* model, const char* path, const char* filename, Shader* shader, bool flipUVs, bool print);