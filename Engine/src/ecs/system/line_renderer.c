#include "pch.h"
#include "line_renderer.h"

#define MAX_LINES 200
#define MAX_VERTICES MAX_LINES * 2

LineRenderer* line_renderer_create(LineRenderer* line_renderer, Assets* assets, Transform transform) {
	line_renderer->transform = transform;

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"layout (location = 1) in vec4 a_color;\n"
		"layout (location = 2) in int a_entity;\n"
		"out vec4 v_color;\n"
		"out flat int v_entity;\n"
		"uniform mat4 u_model;\n"
		"uniform mat4 u_view_projection;\n"
		"void main() {\n"
		"	gl_Position = u_view_projection * vec4(a_pos.xy, -a_pos.z, 1.0);\n"
		"	v_color = a_color;\n"
		"	v_entity = a_entity;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int color2;\n"
		"in vec4 v_color;\n"
		"in flat int v_entity;\n"
		"void main() {\n"
		"	FragColor = v_color;\n"
		"	color2 = v_entity;\n"
		"}\0";

	line_renderer->shader = assets_shader_create(assets, "line_shader", src_vert, src_frag);
	if (line_renderer->shader == NULL) {
		log_error("Failed to create line shader");
		return NULL;
	}

	line_renderer->vertices = m_malloc(MAX_VERTICES * sizeof(LineVertex));
	line_renderer->vertices_count = 0;

	ADataType layout[] = { VEC3F, VEC4F, VEC1I };
	mesh_create(&line_renderer->mesh);
	mesh_init_dynamic(&line_renderer->mesh, MAX_VERTICES * sizeof(LineVertex), NULL, 0, layout, sizeof(layout), P_LINES);

	mesh_set_count(&line_renderer->mesh, 0);

	return line_renderer;
}

void line_renderer_delete(LineRenderer* line_renderer) {
	m_free(line_renderer->vertices, MAX_VERTICES * sizeof(LineVertex));
	mesh_delete(&line_renderer->mesh);
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
	mesh_set_vertices(&line_renderer->mesh, (float*)line_renderer->vertices, line_renderer->vertices_count * sizeof(LineVertex));
}

void line_renderer_render(LineRenderer* line_renderer, mat4* view_projection) {
	shader_bind(line_renderer->shader, view_projection);
	mat4 model = transform_to_mat4(&line_renderer->transform);
	shader_set_model(line_renderer->shader, &model);

	line_renderer_submit(line_renderer);
	mesh_draw_arrays(&line_renderer->mesh);
}