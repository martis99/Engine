#include "pch.h"
#include "mesh_renderer.h"
#include "ecs/component/transform.h"
#include "ecs/component/mesh_component.h"

#include "assets/assets.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "assets/shader.h"

MeshRenderer* mesh_renderer_create(MeshRenderer* mesh_renderer, Renderer* renderer) {
	mesh_renderer->renderer = renderer;

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"layout (location = 1) in vec2 a_tex_coord;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 u_view_projection;\n"
		"};\n"
		"out vec2 v_tex_coord;\n"
		"uniform mat4 u_model;\n"
		"void main() {\n"
		"	gl_Position = u_view_projection * u_model * vec4(a_pos.xy, -a_pos.z, 1.0);\n"
		"	v_tex_coord = a_tex_coord;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int color2;\n"
		"in vec2 v_tex_coord;\n"
		"uniform vec4 u_color;\n"
		"uniform int u_entity;\n"
		"uniform sampler2D u_texture;\n"
		"void main() {\n"
		"	FragColor = texture(u_texture, v_tex_coord) * u_color;\n"
		"	color2 = u_entity;\n"
		"}\0";

	AValue layout[] = {
		{"Position", VEC3F},
		{"TexCoord", VEC2F}
	};

	AValue props[] = {
		{"u_model", MAT4F},
		{"u_entity", VEC1I},
		{"u_color", VEC4F},
	};

	if (shader_create(&mesh_renderer->shader, renderer, src_vert, src_frag, layout, sizeof(layout), props, sizeof(props), "u_texture", 1) == NULL) {
		log_error("Failed to create mesh shader");
		return NULL;
	}

	return mesh_renderer;
}

void mesh_renderer_delete(MeshRenderer* mesh_renderer) {
	shader_delete(&mesh_renderer->shader);
}

void mesh_renderer_render(MeshRenderer* mesh_renderer, Ecs* ecs) {
	shader_bind(&mesh_renderer->shader, mesh_renderer->renderer);

	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_MESH);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		MeshComponent* mesh_component = (MeshComponent*)ecs_get(ecs, qr->list[i], C_MESH);

		mat4 model = transform_to_mat4(transform);
		material_set_value(mesh_component->material, 0, &model);
		Id entity = qr->list[i];
		material_set_value(mesh_component->material, 1, &entity);
		material_bind(mesh_component->material, mesh_renderer->renderer, 1);
		mesh_draw_elements(mesh_component->mesh, mesh_renderer->renderer);
	}
}

Material* mesh_renderer_create_material(MeshRenderer* mesh_renderer, Assets* assets, const char* name, Texture* texture, vec4 color) {
	Material* material = assets_material_create(assets, name, &mesh_renderer->shader);
	material_add_texture(material, texture);
	material_set_value(material, 2, &color);
	return material;
}