#include "gfx_line_renderer.h"

#include "ecs/component/transform.h"
#include "assets/gfx_mesh.h"
#include "assets/gfx_shader.h"
#include "assets/gfx_material.h"

#include "gfx_shader_creator.h"

#define MAX_LINES 200000
#define MAX_VERTICES MAX_LINES * 2

LineRenderer* line_renderer_create(LineRenderer* line_renderer, Renderer* renderer, Transform transform) {
	line_renderer->renderer = renderer;
	line_renderer->transform = transform;

	const char* src_vert =
		"VSOutput vs_main(VSInput vs_input) {\n"
		"	VSOutput vs_output;\n"
		"	vs_output.SV_Position = mul(vec4f(vs_input.Position, 1.0f), mul(Model, ViewProjection));\n"
		"	vs_output.Color = vs_input.Color;\n"
		"	vs_output.Entity = vs_input.Entity;\n"
		"	return vs_output;\n"
		"}\n"
		"\0";

	const char* src_frag =
		"FSOutput fs_main(VSOutput fs_input) {\n"
		"	FSOutput fs_output;\n"
		"	fs_output.FragColor = fs_input.Color;\n"
		"	fs_output.EntityId = fs_input.Entity;\n"
		"	return fs_output;\n"
		"}\n"
		"\0";

	AValue vs_in[] = {
		{VEC3F, "Position"},
		{VEC4F, "Color"},
		{VEC1I, "Entity"}
	};

	AValue vs_out[] = {
		{VEC4F, "SV_Position"},
		{VEC4F, "Color"},
		{VEC1I, "Entity"}
	};

	AValue global[] = {
		{MAT4F, "ViewProjection"}
	};

	AValue vs[] = {
		{MAT4F, "Model"}
	};

	AValue output[] = {
		{VEC4F, "FragColor"},
		{VEC1I, "EntityId"}
	};

	ABufferDesc buffers[] = {
		{A_BFR_VS_IN0, 0, vs_in, sizeof(vs_in), MAX_VERTICES, "VSInput"},
		{A_BFR_VS_OUT, 0, vs_out, sizeof(vs_out), MAX_VERTICES, "VSOutput"},
		{A_BFR_GLOBAL, 0, global, sizeof(global), 0, "Camera"},
		{A_BFR_VS, 1, vs, sizeof(vs), 0, "VSMaterial"},
		{A_BFR_PS_OUT, 0, output, sizeof(output), 0, "FSOutput"}
	};

	AShaderDesc shader_desc = { 0 };
	shader_desc.buffers = buffers;
	shader_desc.buffers_size = sizeof(buffers);

	if (gfx_sc_create_shader(&renderer->shader_creator, &line_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_msg(renderer->log, "Failed to create line shader");
		return NULL;
	}

	if (material_create(&line_renderer->material, renderer, &line_renderer->shader) == NULL) {
		log_msg(renderer->log, "Failed to create line material");
		return NULL;
	}

	line_renderer->vertices = m_malloc(MAX_VERTICES * sizeof(LineVertex));
	line_renderer->vertices_count = 0;

	AMeshData md = { 0 };
	if (mesh_create(&line_renderer->mesh, renderer, &line_renderer->shader, md, A_LINES) == NULL) {
		log_msg(renderer->log, "Failed to create line mesh");
		return NULL;
	}

	return line_renderer;
}

void line_renderer_delete(LineRenderer* line_renderer) {
	m_free(line_renderer->vertices, MAX_VERTICES * sizeof(LineVertex));
	material_delete(&line_renderer->material, line_renderer->renderer);
	mesh_delete(&line_renderer->mesh, line_renderer->renderer);
	shader_delete(&line_renderer->shader, line_renderer->renderer);
}

void line_renderer_clear(LineRenderer* line_renderer) {
	line_renderer->vertices_count = 0;
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