#include "instance_renderer.h"

#include "ecs/component/transform.h"
#include "ecs/component/instance_component.h"

#include "assets/assets.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "assets/shader.h"

#include "ecs/ecs.h"

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
		{VEC3F, "Position"},
		{VEC2F, "TexCoord"}
	};

	AValue instance[] = {
		{MAT4F, "Transform" }
	};

	AValue index[] = { {VEC1UI, ""} };

	AValue global[] = {
		{MAT4F, "ViewProjection"}
	};

	AValue vs[] = {
		{MAT4F, "Model"}
	};

	AValue ps[] = {
		{VEC4F, "Color"},
		{VEC1I, "Entity"}
	};

	AValue output[] = {
		{VEC4F, "FragColor"},
		{VEC1I, "EntityId"}
	};

	ABufferDesc buffers[] = {
		{A_BFR_VERTEX, 0, vertex, sizeof(vertex), 0, "Input"},
		{A_BFR_INSTANCE, 0, instance, sizeof(instance), MAX_INSTANCES, "Input2" },
		{A_BFR_INDEX, 0, index, sizeof(index), 0, ""},
		{A_BFR_GLOBAL, 0, global, sizeof(global), 0, "Camera"},
		{A_BFR_VS, 1, vs, sizeof(vs), 0, "VSMaterial"},
		{A_BFR_PS, 2, ps, sizeof(ps), 0, "PSMaterial"},
		{A_BFR_PS_OUT, 0, output, sizeof(output), 0, "Output"}
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