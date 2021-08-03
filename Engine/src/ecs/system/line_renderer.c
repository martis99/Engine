#include "pch.h"
#include "line_renderer.h"

#include "ecs/component/transform.h"
#include "assets/mesh.h"
#include "assets/shader.h"
#include "assets/material.h"

#define MAX_LINES 200
#define MAX_VERTICES MAX_LINES * 2

LineRenderer* line_renderer_create(LineRenderer* line_renderer, Renderer* renderer, Transform transform) {
	line_renderer->renderer = renderer;
	line_renderer->transform = transform;

#ifdef GAPI_OPENGL
	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec4 Color;\n"
		"layout (location = 2) in int Entity;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 ViewProjection;\n"
		"};\n"
		"uniform mat4 Model;\n"
		"out vec4     VColor;\n"
		"out flat int VEntity;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position.xy, -Position.z, 1.0);\n"
		"	VColor = Color;\n"
		"	VEntity = Entity;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int EntityId;\n"
		"in vec4     VColor;\n"
		"in flat int VEntity;\n"
		"void main() {\n"
		"	FragColor = VColor;\n"
		"	EntityId = VEntity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"cbuffer Camera {\n"
		"	row_major matrix ViewProjection;\n"
		"};\n"
		"cbuffer Object {\n"
		"	row_major matrix Model;\n"
		"};\n"
		"struct Input {\n"
		"	float3 pos       : Position;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"struct Output {\n"
		"	float4 pos       : SV_Position;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos       = mul(float4(input.pos.x, input.pos.y, -input.pos.z, 1.0f), mul(Model, ViewProjection));\n"
		"	output.color     = input.color;\n"
		"	output.entity    = input.entity;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"struct Input {\n"
		"	float4 pos       : SV_Position;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"float4 main(Input input) : SV_TARGET {\n"
		"	return input.color;\n"
		"}\0";
#endif
	AValue layout[] = {
		{"Position", VEC3F},
		{"Color", VEC4F},
		{"Entity", VEC1I}
	};

	AValue props[] = {
		{"Model", MAT4F},
	};

	if (shader_create(&line_renderer->shader, renderer, src_vert, src_frag, layout, sizeof(layout), NULL, 0, props, sizeof(props), "", 1) == NULL) {
		log_error("Failed to create line shader");
		return NULL;
	}

	if (material_create(&line_renderer->material, renderer, &line_renderer->shader) == NULL) {
		log_error("Failed to create line material");
		return NULL;
	}

	line_renderer->vertices = m_malloc(MAX_VERTICES * sizeof(LineVertex));
	line_renderer->vertices_count = 0;

	mesh_create(&line_renderer->mesh);
	mesh_init_dynamic(&line_renderer->mesh, renderer, &line_renderer->shader, MAX_VERTICES * sizeof(LineVertex), sizeof(LineVertex), NULL, 0, 0, A_LINES);

	mesh_set_count(&line_renderer->mesh, 0);

	return line_renderer;
}

void line_renderer_delete(LineRenderer* line_renderer) {
	m_free(line_renderer->vertices, MAX_VERTICES * sizeof(LineVertex));
	material_delete(&line_renderer->material);
	mesh_delete(&line_renderer->mesh);
	shader_delete(&line_renderer->shader);
}

void line_renderer_clear(LineRenderer* line_renderer) {
	line_renderer->vertices_count = 0;
	mesh_set_count(&line_renderer->mesh, 0);
}

void line_renderer_add(LineRenderer* line_renderer, vec3 start, vec3 end, vec4 color, int entity) {
	line_renderer->vertices[line_renderer->vertices_count + 0].position = start;
	line_renderer->vertices[line_renderer->vertices_count + 0].color = color;
	line_renderer->vertices[line_renderer->vertices_count + 0].entity = entity;
	line_renderer->vertices[line_renderer->vertices_count + 1].position = end;
	line_renderer->vertices[line_renderer->vertices_count + 1].color = color;
	line_renderer->vertices[line_renderer->vertices_count + 1].entity = entity;

	mesh_add_count(&line_renderer->mesh, 2);
	line_renderer->vertices_count += 2;
}

void line_renderer_submit(LineRenderer* line_renderer) {
	mesh_set_vertices(&line_renderer->mesh, line_renderer->renderer, line_renderer->vertices, line_renderer->vertices_count * sizeof(LineVertex));
}

void line_renderer_render(LineRenderer* line_renderer) {
	shader_bind(&line_renderer->shader, line_renderer->renderer);
	mat4 model = transform_to_mat4(&line_renderer->transform);
	material_set_value(&line_renderer->material, 0, &model);
	material_upload(&line_renderer->material, line_renderer->renderer);
	material_bind(&line_renderer->material, line_renderer->renderer, 1);

	line_renderer_submit(line_renderer);
	mesh_draw_arrays(&line_renderer->mesh, line_renderer->renderer);
}