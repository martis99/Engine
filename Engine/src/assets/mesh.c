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

void mesh_init_cube(Mesh* mesh) {
	float vertices[] = {
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		 0.5f,  0.5f, 0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	};

	uint indices[] = {
		 0,  1,  3,
		 3,  1,  2,
		 4,  5,  7,
		 7,  5,  6,
		 8,  9, 11,
		11,  9, 10,
		12, 13, 15,
		15, 13, 14,
		16, 17, 19,
		19, 17, 18,
		20, 21, 23,
		23, 21, 22
	};

	uint layout[] = { 3, 2 };
	mesh_init_static(mesh, vertices, sizeof(vertices), indices, sizeof(indices), layout, sizeof(layout));
}