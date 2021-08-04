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

#ifdef GAPI_NONE
	const char* src_vert = "";
	const char* src_frag = "";
#elif GAPI_OPENGL
	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec2 TexCoord;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 ViewProjection;\n"
		"};\n"
		"uniform mat4 Model;\n"
		"out vec2 VTexCoord;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position.xy, -Position.z, 1.0);\n"
		"	VTexCoord = TexCoord;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int  EntityId;\n"
		"in vec2 VTexCoord;\n"
		"uniform vec4 Color;\n"
		"uniform int Entity;\n"
		"uniform sampler2D Textures[4];\n"
		"void main() {\n"
		"	FragColor = texture(Textures[0], VTexCoord) * Color;\n"
		"	EntityId = 0;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"cbuffer Camera {\n"
		"	row_major matrix ViewProjection;\n"
		"};\n"
		"cbuffer Object {\n"
		"	row_major matrix Model;\n"
		"	float4 Color;\n"
		"};\n"
		"struct Input {\n"
		"	float3 pos       : Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"};\n"
		"struct Output {\n"
		"	float4 pos       : SV_Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos       = mul(float4(input.pos.x, input.pos.y, -input.pos.z, 1.0f), mul(Model, ViewProjection));\n"
		"	output.tex_coord = input.tex_coord;\n"
		"	output.color     = Color;\n"
		"	output.entity    = 0;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"Texture2D Textures[4];\n"
		"SamplerState Samplers[4];\n"
		"struct Input {\n"
		"	float4 pos       : SV_Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"float4 tex_color(int tex_id, float2 tex_coord) {\n"
		"	switch (tex_id) {\n"
		"		case 0: return Textures[0].Sample(Samplers[0], tex_coord);\n"
		"		case 1: return Textures[1].Sample(Samplers[1], tex_coord);\n"
		"		case 2: return Textures[2].Sample(Samplers[2], tex_coord);\n"
		"		case 3: return Textures[3].Sample(Samplers[3], tex_coord);\n"
		"	}\n"
		"	return float4(1, 1, 1, 1);\n"
		"}\n"
		"float4 main(Input input) : SV_TARGET {\n"
		"	return input.color * tex_color(0, input.tex_coord);\n"
		"}\0";
#endif

	AValue vertex[] = {
		{"Position", VEC3F},
		{"TexCoord", VEC2F}
	};

	AValue index[] = { {"", VEC1U} };

	AMeshDesc md = { 0 };
	md.vertices.enabled = 1;
	md.vertices.layout = vertex;
	md.vertices.layout_size = sizeof(vertex);
	md.instances.enabled = 0;
	md.instances.layout = NULL;
	md.instances.layout_size = 0;
	md.indices.enabled = 1;
	md.indices.layout = index;
	md.indices.layout_size = sizeof(index);

	AValue props[] = {
		{"Model", MAT4F},
		{"Color", VEC4F}
	};

	if (shader_create(&mesh_renderer->shader, renderer, src_vert, src_frag, md, props, sizeof(props), "Textures", 4) == NULL) {
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
		material_upload(mesh_component->material, mesh_renderer->renderer);
		Id entity = qr->list[i];
		//material_set_value(mesh_component->material, 1, &entity);
		material_bind(mesh_component->material, mesh_renderer->renderer, 1);
		mesh_draw(mesh_component->mesh, mesh_renderer->renderer, 0xFFFFFFFF);
	}
}

Material* mesh_renderer_create_material(MeshRenderer* mesh_renderer, Assets* assets, const char* name, Texture* texture, vec4 color) {
	Material* material = assets_material_create(assets, name, &mesh_renderer->shader);
	material_add_texture(material, texture);
	material_set_value(material, 1, &color);
	return material;
}