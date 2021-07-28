#include "pch.h"
#include "batch_renderer.h"

#include "ecs/component/transform.h"
#include "assets/shader.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "assets/texture.h"

#define MAX_TEXTURES 16
#define MAX_QUADS 200
#define MAX_VERTICES MAX_QUADS * 4
#define MAX_INDICES MAX_QUADS * 6

BatchRenderer* batch_renderer_create(BatchRenderer* batch_renderer, Renderer* renderer, Material* material, ADataType* layout, uint layout_size, size_t vertex_size) {
	batch_renderer->renderer = renderer;
	batch_renderer->shader = material->shader;
	batch_renderer->material = material;

	batch_renderer->vertices = m_malloc(MAX_VERTICES * vertex_size);
	batch_renderer->vertices_count = 0;
	batch_renderer->vertex_size = vertex_size;

	uint indices[MAX_INDICES];

	int offset = 0;
	for (int i = 0; i < MAX_INDICES; i += 6) {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 3;
		indices[i + 2] = offset + 1;

		indices[i + 3] = offset + 3;
		indices[i + 4] = offset + 2;
		indices[i + 5] = offset + 1;

		offset += 4;
	}

	mesh_create(&batch_renderer->mesh);
	mesh_init_dynamic(&batch_renderer->mesh, renderer, batch_renderer->shader, MAX_VERTICES * (uint)vertex_size, indices, MAX_INDICES * sizeof(uint), layout, layout_size, A_TRIANGLES);

	mesh_set_count(&batch_renderer->mesh, 0);

	batch_renderer->textures = m_malloc(MAX_TEXTURES * sizeof(Texture*));
	batch_renderer->textures_count = 0;

	int samplers[MAX_TEXTURES];
	for (int i = 0; i < MAX_TEXTURES; i++) {
		samplers[i] = i;
	}
	material_set_vec1i(material, "u_textures", MAX_TEXTURES, samplers);

	return batch_renderer;
}

void batch_renderer_delete(BatchRenderer* batch_renderer) {
	m_free(batch_renderer->vertices, MAX_VERTICES * batch_renderer->vertex_size);
	m_free(batch_renderer->textures, MAX_TEXTURES * sizeof(Texture*));
	mesh_delete(&batch_renderer->mesh);
}

void batch_renderer_clear(BatchRenderer* batch_renderer) {
	batch_renderer->vertices_count = 0;
	batch_renderer->textures_count = 0;
	mesh_set_count(&batch_renderer->mesh, 0);
}

static uint add_texture(BatchRenderer* batch_renderer, Texture* texture) {
	for (uint i = 0; i < batch_renderer->textures_count; i++) {
		if (batch_renderer->textures[i] == texture) {
			return i;
		}
	}

	batch_renderer->textures[batch_renderer->textures_count] = texture;
	batch_renderer->textures_count++;
	return batch_renderer->textures_count - 1;
}

static void add_quad(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec2* tex_coords, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*)) {
	vec3 vertices[] = {
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f }
	};

	uint tex_index = add_texture(batch_renderer, texture);
	for (int i = 0; i < 4; i++) {
		int index = batch_renderer->vertices_count + i;
		void* vertex = (byte*)batch_renderer->vertices + index * batch_renderer->vertex_size;
		add_vertex(vertex, transform_vec3(transform, vertices[i]), tex_coords[i], tex_index, data);
	}

	mesh_add_count(&batch_renderer->mesh, 6);
	batch_renderer->vertices_count += 4;
}

void batch_renderer_add(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*)) {
	vec2 tex_coords[] = {
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f}
	};

	add_quad(batch_renderer, transform, texture, tex_coords, data, add_vertex);
}

void batch_renderer_add_sub(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec2i pos, vec2i size, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*)) {
	vec2 min = (vec2){ (float)pos.x / texture->width, (float)pos.y / texture->height };
	vec2 max = (vec2){ ((float)pos.x + size.x) / texture->width, ((float)pos.y + size.y) / texture->height };
	vec2 tex_coords[] = {
		{min.x ,max.y},
		{max.x, max.y},
		{max.x, min.y},
		{min.x, min.y}
	};

	add_quad(batch_renderer, transform, texture, tex_coords, data, add_vertex);
}

void batch_renderer_submit(BatchRenderer* batch_renderer) {
	mesh_set_vertices(&batch_renderer->mesh, batch_renderer->vertices, batch_renderer->vertices_count * (uint)batch_renderer->vertex_size);
}

void batch_renderer_draw(Transform* transform, BatchRenderer* batch_renderer) {
	shader_bind(batch_renderer->shader, batch_renderer->renderer);
	mat4 model = transform_to_mat4(transform);
	shader_set_model(batch_renderer->shader, &model);
	material_bind(batch_renderer->material, batch_renderer->renderer);

	for (uint i = 0; i < batch_renderer->textures_count; i++) {
		texture_bind(batch_renderer->textures[i], batch_renderer->renderer, i);
	}
	mesh_draw_elements(&batch_renderer->mesh, batch_renderer->renderer);
}