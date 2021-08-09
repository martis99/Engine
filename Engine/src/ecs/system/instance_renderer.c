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
		"out Vertex {\n"
		"	vec2 TexCoord;\n"
		"} vs_out;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * Transform * vec4(Position.xy, -Position.z, 1.0);\n"
		"	vs_out.TexCoord = TexCoord;\n"
		"}\0";

	const char* src_frag =
		"in Vertex {\n"
		"	vec2 TexCoord;\n"
		"} vs_in;\n"
		"void main() {\n"
		"	FragColor = sample_tex(0, vs_in.TexCoord) * Color;\n"
		"	EntityId = Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"struct Output {\n"
		"	float4 pos       : SV_Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"};\n"
		"Output main(Input input, Input2 input2) {\n"
		"	Output output;\n"
		"	output.pos       = mul(float4(input.Position.x, input.Position.y, -input.Position.z, 1.0f), mul(input2.Transform, mul(Model, ViewProjection)));\n"
		"	output.tex_coord = input.TexCoord;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"struct Input {\n"
		"	float4 pos       : SV_Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.FragColor = Color * sample_tex(0, input.tex_coord);\n"
		"	output.EntityId = Entity;\n"
		"	return output;\n"
		"}\0";
#endif

	AValue vertex[] = {
		{"Position", VEC3F},
		{"TexCoord", VEC2F}
	};

	AValue instance[] = {
		{"Transform", MAT4F }
	};

	AValue index[] = { {"", VEC1UI} };

	AValue global[] = {
		{"ViewProjection", MAT4F}
	};

	AValue vs[] = {
		{"Model", MAT4F}
	};

	AValue ps[] = {
		{"Color", VEC4F},
		{"Entity", VEC1I}
	};

	AValue output[] = {
		{"FragColor", VEC4F},
		{"EntityId", VEC1I}
	};

	ABufferDesc buffers[] = {
		{A_BFR_VERTEX, "Input", 0, vertex, sizeof(vertex)},
		{A_BFR_INSTANCE, "Input2", 0, instance, sizeof(instance), MAX_INSTANCES },
		{A_BFR_INDEX, NULL, 0, index, sizeof(index)},
		{A_BFR_GLOBAL, "Camera", 0, global, sizeof(global)},
		{A_BFR_VS, "VSMaterial", 1, vs, sizeof(vs)},
		{A_BFR_PS, "PSMaterial", 2, ps, sizeof(ps)},
		{A_BFR_PS_OUT, "Output", 0, output, sizeof(output)}
	};

	AShaderDesc shader_desc = { 0 };
	shader_desc.buffers = buffers;
	shader_desc.buffers_size = sizeof(buffers);
	shader_desc.textures_count = 2;
	shader_desc.texture_type = VEC4F;

	if (shader_create(&instance_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
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
		material_set_vs_value(instance_component->material, 0, &model);
		material_set_ps_value(instance_component->material, 1, &qr->list[i]);
		material_upload(instance_component->material, instance_renderer->renderer);
		material_bind(instance_component->material, instance_renderer->renderer);
		mesh_set_instances(instance_component->mesh, instance_renderer->renderer, instance_component->transforms, instance_component->transforms_count * sizeof(mat4));
		mesh_draw(instance_component->mesh, instance_renderer->renderer, 0xFFFFFFFF);
	}
}

Material* instance_renderer_create_material(InstanceRenderer* instance_renderer, Assets* assets, const char* name, Texture* texture, vec4 color) {
	Material* material = assets_material_create(assets, name, &instance_renderer->shader);
	material_add_texture(material, texture);
	material_set_ps_value(material, 0, &color);
	return material;
}