#include "pch.h"
#include "batch_renderer.h"

#define MAX_TEXTURES 16
#define MAX_QUADS 200
#define MAX_VERTICES MAX_QUADS * 4
#define MAX_INDICES MAX_QUADS * 6

BatchRenderer* batch_renderer_create(BatchRenderer* batch_renderer, Material* material) {
	batch_renderer->shader = material->shader;
	batch_renderer->material = material;

	batch_renderer->vertices = m_malloc(MAX_VERTICES * sizeof(Vertex));
	batch_renderer->vertices_count = 0;

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

	uint layout[] = { 3, 4, 2, 1, 2, 4 };
	mesh_create(&batch_renderer->mesh);
	mesh_init_dynamic(&batch_renderer->mesh, MAX_VERTICES * sizeof(Vertex), indices, MAX_INDICES * sizeof(uint), layout, sizeof(layout), P_TRIANGLES);

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
	m_free(batch_renderer->vertices, MAX_VERTICES * sizeof(Vertex));
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

static void add_quad(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec4 color, vec2* tex_coords, vec4 borders) {
	vec3 vertices[] = {
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f }
	};

	uint tex_index = add_texture(batch_renderer, texture);
	for (int i = 0; i < 4; i++) {
		batch_renderer->vertices[batch_renderer->vertices_count + i].position = transform_vec3(transform, vertices[i]);
		batch_renderer->vertices[batch_renderer->vertices_count + i].color = color;
		batch_renderer->vertices[batch_renderer->vertices_count + i].tex_coord = tex_coords[i];
		batch_renderer->vertices[batch_renderer->vertices_count + i].tex_index = (float)tex_index;
		batch_renderer->vertices[batch_renderer->vertices_count + i].size = vec3_to_vec2(transform->scale);
		batch_renderer->vertices[batch_renderer->vertices_count + i].borders = borders;
	}

	mesh_add_count(&batch_renderer->mesh, 6);
	batch_renderer->vertices_count += 4;
}

void batch_renderer_add(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec4 color, vec4 borders) {
	vec2 tex_coords[] = {
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f}
	};

	add_quad(batch_renderer, transform, texture, color, tex_coords, borders);
}

void batch_renderer_add_sub(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec4 color, vec2i pos, vec2i size) {
	vec2 min = (vec2){ (float)pos.x / texture->width, (float)pos.y / texture->height };
	vec2 max = (vec2){ ((float)pos.x + size.x) / texture->width, ((float)pos.y + size.y) / texture->height };
	vec2 tex_coords[] = {
		{min.x ,max.y},
		{max.x, max.y},
		{max.x, min.y},
		{min.x, min.y}
	};

	add_quad(batch_renderer, transform, texture, color, tex_coords, (vec4) { 0, 0, 0, 0 });
}

void batch_renderer_submit(BatchRenderer* batch_renderer) {
	mesh_set_vertices(&batch_renderer->mesh, (float*)batch_renderer->vertices, batch_renderer->vertices_count * sizeof(Vertex));
}

void batch_renderer_draw(Transform* transform, BatchRenderer* batch_renderer, mat4* view_projection) {
	shader_bind(batch_renderer->shader, view_projection);
	mat4 model = transform_to_mat4(transform);
	shader_set_model(batch_renderer->shader, &model);
	material_bind(batch_renderer->material);

	for (uint i = 0; i < batch_renderer->textures_count; i++) {
		texture_bind(batch_renderer->textures[i], i);
	}
	mesh_draw(&batch_renderer->mesh);
}