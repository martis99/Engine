#include "pch.h"
#include "mesh.h"

#include "app.h"

Mesh* mesh_create(Mesh* mesh) {
	mesh->mesh = amesh_create();
	return mesh;
}

void mesh_delete(Mesh* mesh) {
	amesh_delete(mesh->mesh);
}

void mesh_init_static(Mesh* mesh, float* vertices, uint vertices_size, uint* indices, uint indices_size, uint* layout, uint layout_size) {
	amesh_init_static(mesh->mesh, vertices, vertices_size, indices, indices_size, layout, layout_size);
}

void mesh_draw(Mesh* mesh) {
	amesh_draw(mesh->mesh);
	app.stats.draw_calls++;
}

void mesh_init_quad(Mesh* mesh) {
	float vertices[] = {
		0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
	};

	uint indices[] = {
		0, 3, 1,
		3, 2, 1
	};

	uint layout[] = { 3, 2 };
	mesh_init_static(mesh, vertices, sizeof(vertices), indices, sizeof(indices), layout, sizeof(layout));
}