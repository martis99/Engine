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

#ifdef GAPI_NONE
	const char* src_vert = "";
	const char* src_frag = "";
#elif GAPI_OPENGL
	const char* src_vert =
		"out Vertex {\n"
		"	vec4 Color;\n"
		"	flat int Entity;\n"
		"} vs_out;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position.xy, -Position.z, 1.0);\n"
		"	vs_out.Color = Color;\n"
		"	vs_out.Entity = Entity;\n"
		"}\0";

	const char* src_frag =
		"in Vertex {\n"
		"	vec4 Color;\n"
		"	flat int Entity;\n"
		"} vs_in;\n"
		"void main() {\n"
		"	Color = vs_in.Color;\n"
		"	EntityId = vs_in.Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"struct Output {\n"
		"	float4 pos       : SV_Position;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos       = mul(float4(input.Position.x, input.Position.y, -input.Position.z, 1.0f), mul(Model, ViewProjection));\n"
		"	output.color     = input.Color;\n"
		"	output.entity    = input.Entity;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"struct Input {\n"
		"	float4 pos       : SV_Position;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.Color = input.color;\n"
		"	output.EntityId = input.entity;\n"
		"	return output;\n"
		"}\0";
#endif
	AValue vertex[] = {
		{"Position", VEC3F},
		{"Color", VEC4F},
		{"Entity", VEC1I}
	};

	AValue global[] = {
		{"ViewProjection", MAT4F}
	};

	AValue vs[] = {
		{"Model", MAT4F}
	};

	AValue output[] = {
		{"Color", VEC4F},
		{"EntityId", VEC1I}
	};

	ABufferDesc buffers[] = {
		{A_BFR_VERTEX, "Input", 0, vertex, sizeof(vertex), MAX_VERTICES},
		{A_BFR_GLOBAL, "Camera", 0, global, sizeof(global)},
		{A_BFR_VS, "VSMaterial", 1, vs, sizeof(vs)},
		{A_BFR_PS_OUT, "Output", 0, output, sizeof(output)}
	};

	AShaderDesc shader_desc = { 0 };
	shader_desc.buffers = buffers;
	shader_desc.buffers_size = sizeof(buffers);

	if (shader_create(&line_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_error("Failed to create line shader");
		return NULL;
	}

	if (material_create(&line_renderer->material, renderer, &line_renderer->shader) == NULL) {
		log_error("Failed to create line material");
		return NULL;
	}

	line_renderer->vertices = m_malloc(MAX_VERTICES * sizeof(LineVertex));
	line_renderer->vertices_count = 0;

	AMeshData md = { 0 };
	if (mesh_create(&line_renderer->mesh, renderer, &line_renderer->shader, md, A_LINES) == NULL) {
		log_error("Failed to create line mesh");
		return NULL;
	}

	return line_renderer;
}

void line_renderer_delete(LineRenderer* line_renderer) {
	m_free(line_renderer->vertices, MAX_VERTICES * sizeof(LineVertex));
	material_delete(&line_renderer->material);
	mesh_delete(&line_renderer->mesh);
	shader_delete(&line_renderer->shader);
}

void line_renderer_add(LineRenderer* line_renderer, vec3 start, vec3 end, vec4 color, int entity) {
	line_renderer->vertices[line_renderer->vertices_count + 0].position = start;
	line_renderer->vertices[line_renderer->vertices_count + 0].color = color;
	line_renderer->vertices[line_renderer->vertices_count + 0].entity = entity;
	line_renderer->vertices[line_renderer->vertices_count + 1].position = end;
	line_renderer->vertices[line_renderer->vertices_count + 1].color = color;
	line_renderer->vertices[line_renderer->vertices_count + 1].entity = entity;

	line_renderer->vertices_count += 2;
}

void line_renderer_render(LineRenderer* line_renderer) {
	mesh_set_vertices(&line_renderer->mesh, line_renderer->renderer, line_renderer->vertices, line_renderer->vertices_count * sizeof(LineVertex));

	shader_bind(&line_renderer->shader, line_renderer->renderer);
	mat4 model = transform_to_mat4(&line_renderer->transform);
	material_set_vs_value(&line_renderer->material, 0, &model);
	material_upload(&line_renderer->material, line_renderer->renderer);
	material_bind(&line_renderer->material, line_renderer->renderer);

	mesh_draw(&line_renderer->mesh, line_renderer->renderer, 0xFFFFFFFF);
}