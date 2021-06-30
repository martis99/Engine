#pragma once
#include "assets/mesh.h"
#include "assets/material.h"

typedef struct MeshComponent {
	Mesh mesh;
	Material* material;
} MeshComponent;

MeshComponent mesh_component_create(Mesh* mesh, Material* material);