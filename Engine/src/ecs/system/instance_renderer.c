#include "pch.h"
#include "instance_renderer.h"

#include "ecs/component/transform.h"
#include "ecs/component/instance_component.h"

#include "assets/assets.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "assets/shader.h"

#define MAX_INSTANCES 200

InstanceRenderer* instance_renderer_create(InstanceRenderer* instance_renderer, Renderer* renderer) {
	instance_renderer->renderer = renderer;

#ifdef GAPI_NONE
	const char* src_vert = "";
	const char* src_frag = "";
#elif GAPI_OPENGL
	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec2 TexCoord;\n"
		"layout (location = 2) in mat4 Transform;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 ViewProjection;\n"
		"};\n"
		"uniform mat4 Model;\n"
		"out vec2 VTexCoord;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * Transform * vec4(Position.xy, -Position.z, 1.0);\n"
		"	VTexCoord = TexCoord;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"uniform sampler2D Texture;\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int EntityId;\n"
		"in vec2 VTexCoord;\n"
		"uniform vec4 Color;\n"
		"uniform int Entity;\n"
		"void main() {\n"
		"	FragColor = texture(Texture, VTexCoord) * Color;\n"
		"	EntityId = Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"cbuffer Camera {\n"
		"	row_major matrix ViewProjection;\n"
		"};\n"
		"cbuffer Object {\n"
		"	row_major matrix Model;\n"
		"	float4 Color;\n"
		"	int Entity;\n"
		"};\n"
		"struct Input {\n"
		"	float3 pos  : Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"	row_major matrix transform : Transform;\n"
		"};\n"
		"struct Output {\n"
		"	float4 pos       : SV_Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"	float4 color     : Color;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos       = mul(float4(input.pos.x, input.pos.y, -input.pos.z, 1.0f), mul(input.transform, mul(Model, ViewProjection)));\n"
		"	output.tex_coord = input.tex_coord;\n"
		"	output.color     = Color;\n"
		"	output.entity    = Entity;\n"
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

	AValue instance[] = {
		{"Transform", MAT4F }
	};

	AValue index[] = { {"", VEC1U} };

	AMeshDesc md = { 0 };
	md.vertices.enabled = 1;
	md.vertices.layout = vertex;
	md.vertices.layout_size = sizeof(vertex);
	md.instances.enabled = 1;
	md.instances.layout = instance;
	md.instances.layout_size = sizeof(instance);
	md.indices.enabled = 1;
	md.indices.layout = index;
	md.indices.layout_size = sizeof(index);

	md.instances.data_size = MAX_INSTANCES * abufferdesc_size(md.instances);

	AValue props[] = {
		{"Model", MAT4F},
		{"Color", VEC4F},
		{"Entity", VEC1I},
	};

	if (shader_create(&instance_renderer->shader, renderer, src_vert, src_frag, md, props, sizeof(props), "Textures", 4) == NULL) {
		log_error("Failed to create mesh shader");
		return NULL;
	}
	return instance_renderer;
}

void instance_renderer_delete(InstanceRenderer* instance_renderer, Ecs* ecs) {
	QueryResult* qr = ecs_query(ecs, 1, C_INSTANCE);
	for (uint i = 0; i < qr->count; ++i) {
		instance_component_delete(ecs_get(ecs, qr->list[i], C_INSTANCE));
	}
	shader_delete(&instance_renderer->shader);
}

void instance_renderer_render(InstanceRenderer* instance_renderer, Ecs* ecs) {
	shader_bind(&instance_renderer->shader, instance_renderer->renderer);

	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_INSTANCE);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		InstanceComponent* instance_component = (InstanceComponent*)ecs_get(ecs, qr->list[i], C_INSTANCE);

		mat4 model = transform_to_mat4(transform);
		material_set_value(instance_component->material, 0, &model);
		Id entity = qr->list[i];
		material_set_value(instance_component->material, 2, &entity);
		material_upload(instance_component->material, instance_renderer->renderer);
		material_bind(instance_component->material, instance_renderer->renderer, 1);
		mesh_set_instances(instance_component->mesh, instance_renderer->renderer, instance_component->transforms, instance_component->transforms_count * sizeof(mat4));
		mesh_draw(instance_component->mesh, instance_renderer->renderer, 0xFFFFFFFF);
	}
}

Material* instance_renderer_create_material(InstanceRenderer* instance_renderer, Assets* assets, const char* name, Texture* texture, vec4 color) {
	Material* material = assets_material_create(assets, name, &instance_renderer->shader);
	material_add_texture(material, texture);
	material_set_value(material, 1, &color);
	return material;
}