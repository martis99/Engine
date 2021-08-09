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
		"out Vertex {\n"
		"	vec2 TexCoord;\n"
		"} vs_out;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position.xy, -Position.z, 1.0);\n"
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
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos       = mul(float4(input.Position.x, input.Position.y, -input.Position.z, 1.0f), mul(Model, ViewProjection));\n"
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

	if (shader_create(&mesh_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
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
		material_set_vs_value(mesh_component->material, 0, &model);
		material_set_ps_value(mesh_component->material, 1, &qr->list[i]);
		material_upload(mesh_component->material, mesh_renderer->renderer);
		material_bind(mesh_component->material, mesh_renderer->renderer);
		mesh_draw(mesh_component->mesh, mesh_renderer->renderer, 0xFFFFFFFF);
	}
}

Material* mesh_renderer_create_material(MeshRenderer* mesh_renderer, Assets* assets, const char* name, Texture* texture, vec4 color) {
	Material* material = assets_material_create(assets, name, &mesh_renderer->shader);
	material_add_texture(material, texture);
	material_set_ps_value(material, 0, &color);
	return material;
}