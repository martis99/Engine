#include "gfx_batch_renderer.h"

#include "assets/gfx_material.h"
#include "assets/gfx_mesh.h"
#include "assets/gfx_shader.h"
#include "assets/gfx_texture.h"
#include "ecs/component/transform.h"

#define MAX_QUADS    200
#define MAX_VERTICES MAX_QUADS * 4
#define MAX_INDICES  MAX_QUADS * 6

BatchRenderer *batch_renderer_create(BatchRenderer *batch_renderer, Renderer *renderer, Material *material)
{
	batch_renderer->renderer = renderer;
	batch_renderer->shader	 = material->shader;
	batch_renderer->material = material;

	ABufferDesc *vertices_desc     = ashaderdesc_get_bufferdesc(material->shader->desc, A_BFR_VS_IN0);
	batch_renderer->vertex_size    = abufferdesc_size(vertices_desc);
	batch_renderer->vertices       = m_malloc(MAX_VERTICES * (size_t)batch_renderer->vertex_size);
	batch_renderer->vertices_count = 0;

	batch_renderer->indices_count = 0;

	uint indices[MAX_INDICES] = { 0 };

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

	AMeshData md = {
		.indices.data = indices,
		.indices.size = sizeof(indices),
	};

	if (mesh_create(&batch_renderer->mesh, renderer, material->shader, md, A_TRIANGLES) == NULL) {
		log_error("failed to create batch renderer mesh");
		return NULL;
	}

	return batch_renderer;
}

void batch_renderer_delete(BatchRenderer *batch_renderer)
{
	m_free(batch_renderer->vertices, MAX_VERTICES * (size_t)batch_renderer->vertex_size);
	mesh_delete(&batch_renderer->mesh, batch_renderer->renderer);
}

void batch_renderer_begin(BatchRenderer *batch_renderer)
{
	batch_renderer->vertices_count = 0;
	batch_renderer->indices_count  = 0;
}

static void add_quad(BatchRenderer *batch_renderer, Transform *transform, Texture *texture, vec2 *tex_coords, void *data,
		     void (*add_vertex)(void *, vec3, vec2, int, void *))
{
	vec3 vertices[] = {
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
	};

	uint tex_index = material_add_texture(batch_renderer->material, texture);
	for (int i = 0; i < 4; i++) {
		int index    = batch_renderer->vertices_count + i;
		void *vertex = (byte *)batch_renderer->vertices + index * (size_t)batch_renderer->vertex_size;
		add_vertex(vertex, transform_vec3(transform, vertices[i]), tex_coords[i], tex_index, data);
	}

	batch_renderer->vertices_count += 4;
	batch_renderer->indices_count += 6;
}

void batch_renderer_add(BatchRenderer *batch_renderer, Transform *transform, Texture *texture, void *data, void (*add_vertex)(void *, vec3, vec2, int, void *))
{
	vec2 tex_coords[] = {
		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
	};

	add_quad(batch_renderer, transform, texture, tex_coords, data, add_vertex);
}

void batch_renderer_add_sub(BatchRenderer *batch_renderer, Transform *transform, Texture *texture, vec2i pos, vec2i size, void *data,
			    void (*add_vertex)(void *, vec3, vec2, int, void *))
{
	vec2 min = (vec2){ (float)pos.x / texture->width, (float)pos.y / texture->height };
	vec2 max = (vec2){ ((float)pos.x + size.x) / texture->width, ((float)pos.y + size.y) / texture->height };

	vec2 tex_coords[] = {
		{ min.x, max.y },
		{ max.x, max.y },
		{ max.x, min.y },
		{ min.x, min.y },
	};

	add_quad(batch_renderer, transform, texture, tex_coords, data, add_vertex);
}

void batch_renderer_end(Transform *transform, BatchRenderer *batch_renderer)
{
	mesh_set_vertices(&batch_renderer->mesh, batch_renderer->renderer, batch_renderer->vertices, batch_renderer->vertices_count * (uint)batch_renderer->vertex_size);

	shader_bind(batch_renderer->shader, batch_renderer->renderer);
	mat4 model = transform_to_mat4(transform);
	material_set_vs_value(batch_renderer->material, 0, &model);
	material_upload(batch_renderer->material, batch_renderer->renderer);
	material_bind(batch_renderer->material, batch_renderer->renderer);
	mesh_draw(&batch_renderer->mesh, batch_renderer->renderer, batch_renderer->indices_count);
}
