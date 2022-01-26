#include "gfx_model_renderer.h"
#include "ecs/component/transform.h"

#include "assets/gfx_shader.h"
#include "assets/gfx_model.h"

#include "gfx_shader_creator.h"

ModelRenderer* model_renderer_create(ModelRenderer* model_renderer, Renderer* renderer) {
	model_renderer->renderer = renderer;

	const char* src_vert =
		"VSOutput vs_main(VSInput vs_input) {\n"
		"	VSOutput vs_output;\n"
		"	vs_output.SV_Position = mul(vec4f(vs_input.Position.x, vs_input.Position.y, -vs_input.Position.z, 1.0f), mul(Model, ViewProjection));\n"
		"	vs_output.TexCoord = vs_input.TexCoord;\n"
		"	return vs_output;\n"
		"}\n"
		"\0";

	const char* src_frag =
		"FSOutput fs_main(VSOutput fs_input) {\n"
		"	FSOutput fs_output;\n"
		"	vec4f diffuse = Diffuse * sample(Textures, 0, fs_input.TexCoord);\n"
		"	vec4f specular = Specular * sample(Textures, 1, fs_input.TexCoord);\n"
		"	vec4f col = diffuse + specular;\n"
		"	fs_output.FragColor = vec4f(col.x, col.y, col.z, 1.0f);\n"
		"	fs_output.EntityId = Entity;\n"
		"	return fs_output;\n"
		"}\n"
		"\0";

	AValue vs_in[] = {
		{VEC3F, "Position"},
		{VEC2F, "TexCoord"}
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
		{VEC4F, "Diffuse"},
		{VEC4F, "Specular"},
		{VEC1I, "Entity"}
	};

	AValue output[] = {
		{VEC4F, "FragColor"},
		{VEC1I, "EntityId"}
	};

	ABufferDesc buffers[] = {
		{A_BFR_VS_IN0, 0, vs_in, sizeof(vs_in), 0, "VSInput"},
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

	if (gfx_sc_create_shader(&renderer->shader_creator, &model_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_msg(renderer->log, "Failed to create model shader");
		return NULL;
	}

	return model_renderer;
}

void model_renderer_delete(ModelRenderer* model_renderer) {
	shader_delete(&model_renderer->shader, model_renderer->renderer);
}

void model_renderer_begin(ModelRenderer* model_renderer) {
	shader_bind(&model_renderer->shader, model_renderer->renderer);
}

void model_renderer_render(ModelRenderer* model_renderer, int id, Transform* transform, Model* model) {
	mat4 mat = transform_to_mat4(transform);
	model_draw(model, model_renderer->renderer, &model_renderer->shader, mat, id);
}

void model_renderer_end(ModelRenderer* model_renderer) {

}