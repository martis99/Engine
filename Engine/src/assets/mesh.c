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

void mesh_init_static(Mesh* mesh, float* vertices, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	amesh_init_static(mesh->mesh, vertices, vertices_size, indices, indices_size, layout, layout_size, primitive);
}

void mesh_init_dynamic(Mesh* mesh, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	amesh_init_dynamic(mesh->mesh, vertices_size, indices, indices_size, layout, layout_size, primitive);
}

void mesh_set_vertices(Mesh* mesh, float* vertices, uint vertices_size) {
	amesh_set_vertices(mesh->mesh, vertices, vertices_size);
}

void mesh_set_indices(Mesh* mesh, uint* indices, uint indices_size) {
	amesh_set_indices(mesh->mesh, indices, indices_size);
}

void mesh_draw_arrays(Mesh* mesh) {
	amesh_draw_arrays(mesh->mesh);
	app.stats.draw_calls++;
}

void mesh_draw(Mesh* mesh) {
	amesh_draw(mesh->mesh);
	app.stats.draw_calls++;
}

void mesh_set_count(Mesh* mesh, int count) {
	amesh_set_count(mesh->mesh, count);
}

void mesh_add_count(Mesh* mesh, int count) {
	amesh_add_count(mesh->mesh, count);
}

void mesh_init_quad(Mesh* mesh) {
	float vertices[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	uint indices[] = {
		0, 3, 1,
		3, 2, 1
	};

	ADataType layout[] = { VEC3F, VEC2F };
	mesh_init_static(mesh, vertices, sizeof(vertices), indices, sizeof(indices), layout, sizeof(layout), P_TRIANGLES);
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

	ADataType layout[] = { VEC3F, VEC2F };
	mesh_init_static(mesh, vertices, sizeof(vertices), indices, sizeof(indices), layout, sizeof(layout), P_TRIANGLES);
}