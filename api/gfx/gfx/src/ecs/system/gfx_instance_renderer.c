#include "gfx_instance_renderer.h"

#include "ecs/component/transform.h"

#include "assets/gfx_mesh.h"
#include "assets/gfx_material.h"
#include "assets/gfx_shader.h"

#include "gfx_shader_creator.h"

#define MAX_INSTANCES 200

InstanceRenderer* instance_renderer_create(InstanceRenderer* instance_renderer, Renderer* renderer) {
	instance_renderer->renderer = renderer;

	const char* src_vert =
		"VSOutput vs_main(VSInput0 vs_input0, VSInput1 vs_input1) {\n"
		"	VSOutput vs_output;\n"
		"	vs_output.SV_Position = mul(vec4f(vs_input0.Position.x, vs_input0.Position.y, -vs_input0.Position.z, 1.0f), mul(vs_input1.Transform, mul(Model, ViewProjection)));\n"
		"	vs_output.TexCoord = vs_input0.TexCoord;\n"
		"	return vs_output;\n"
		"}\n"
		"\0";

	const char* src_frag =
		"FSOutput fs_main(VSOutput fs_input) {\n"
		"	FSOutput fs_output;\n"
		"	fs_output.FragColor = Color * sample(Textures, 0, fs_input.TexCoord);\n"
		"	fs_output.EntityId = Entity;\n"
		"	return fs_output;\n"
		"}\n"
		"\0";

	AValue vs_in0[] = {
		{VEC3F, "Position"},
		{VEC2F, "TexCoord"}
	};

	AValue vs_in1[] = {
		{MAT4F, "Transform" }
	};

	AValue vs_out[] = {
		{VEC4F, "SV_Position"},
		{VEC2F, "TexCoord"}
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
		{A_BFR_VS_IN0, 0, vs_in0, sizeof(vs_in0), 0, "VSInput0"},
		{A_BFR_VS_IN1, 0, vs_in1, sizeof(vs_in1), MAX_INSTANCES, "VSInput1" },
		{A_BFR_VS_OUT, 0, vs_out, sizeof(vs_out), 0, "VSOutput"},
		{A_BFR_INDEX, 0, index, sizeof(index), 0, ""},
		{A_BFR_GLOBAL, 0, global, sizeof(global), 0, "Camera"},
		{A_BFR_VS, 1, vs, sizeof(vs), 0, "VSMaterial"},
		{A_BFR_PS, 2, ps, sizeof(ps), 0, "PSMaterial"},
		{A_BFR_PS_OUT, 0, output, sizeof(output), 0, "FSOutput"}
	};

	AShaderDesc shader_desc = { 0 };
	shader_desc.buffers = buffers;
	shader_desc.buffers_size = sizeof(buffers);
	shader_desc.textures_count = 2;
	shader_desc.texture_type = VEC4F;

	if (gfx_sc_create_shader(&renderer->shader_creator, &instance_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_msg(renderer->log, "Failed to create mesh shader");
		return NULL;
	}
	return instance_renderer;
}

void instance_renderer_delete(InstanceRenderer* instance_renderer) {
	shader_delete(&instance_renderer->shader, instance_renderer->renderer);
}

void instance_renderer_begin(InstanceRenderer* instance_renderer) {
	shader_bind(&instance_renderer->shader, instance_renderer->renderer);
}

void instance_renderer_render(InstanceRenderer* instance_renderer, int id, Transform* transform, InstanceComponent* instance_component) {
	mat4 model = transform_to_mat4(transform);
	material_set_vs_value(instance_component->material, 0, &model);
	material_set_ps_value(instance_component->material, 1, &id);
	material_upload(instance_component->material, instance_renderer->renderer);
	material_bind(instance_component->material, instance_renderer->renderer);
	mesh_set_instances(instance_component->mesh, instance_renderer->renderer, instance_component->transforms, instance_component->transforms_count * sizeof(mat4));
	mesh_draw(instance_component->mesh, instance_renderer->renderer, 0xFFFFFFFF);
}

void instance_renderer_end(InstanceRenderer* instance_renderer) {

}