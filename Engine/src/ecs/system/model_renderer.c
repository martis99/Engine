#include "pch.h"
#include "model_renderer.h"
#include "ecs/component/transform.h"
#include "ecs/component/mesh_component.h"

#include "assets/shader.h"
#include "assets/model.h"

ModelRenderer* model_renderer_create(ModelRenderer* model_renderer, Renderer* renderer) {
	model_renderer->renderer = renderer;

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
		"	vec2 TexCoord; \n"
		"} ps_in;\n"
		"void main() {\n"
		"	vec4 diffuse = Diffuse * sample_tex(0, ps_in.TexCoord);\n"
		"	vec4 specular = Specular * sample_tex(1, ps_in.TexCoord);\n"
		"	vec4 col = diffuse + specular;\n"
		"	FragColor = vec4(col.xyz, 1.0);\n"
		"	EntityId = Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"struct Output {\n"
		"	float4 Position : SV_Position;\n"
		"	float2 TexCoord : TexCoord;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.Position = mul(float4(input.Position.x, input.Position.y, -input.Position.z, 1.0f), mul(Model, ViewProjection));\n"
		"	output.TexCoord = input.TexCoord;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"struct Input {\n"
		"	float4 Position : SV_Position;\n"
		"	float2 TexCoord : TexCoord;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	float4 diffuse = Diffuse * sample_tex(0, input.TexCoord);\n"
		"	float4 specular = Specular * sample_tex(1, input.TexCoord);\n"
		"	float4 col = diffuse + specular;\n"
		"	output.FragColor = float4(col.x, col.y, col.z, 1.0);\n"
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
		{"Diffuse", VEC4F},
		{"Specular", VEC4F},
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

	if (shader_create(&model_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_error("Failed to create model shader");
		return NULL;
	}

	return model_renderer;
}

void model_renderer_delete(ModelRenderer* model_renderer) {
	shader_delete(&model_renderer->shader);
}

void model_renderer_render(ModelRenderer* model_renderer, Ecs* ecs) {
	shader_bind(&model_renderer->shader, model_renderer->renderer);

	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_MODEL);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Model* model = (Model*)ecs_get(ecs, qr->list[i], C_MODEL);

		mat4 mat = transform_to_mat4(transform);
		model_draw(model, model_renderer->renderer, &model_renderer->shader, mat, qr->list[i]);
	}
}