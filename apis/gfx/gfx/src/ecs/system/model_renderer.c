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
		"struct Output {\n"
		"	vec4 Position;\n"
		"	vec2 TexCoord;\n"
		"};\n"
		"struct Input {\n"
		"	vec3 Position;\n"
		"	vec2 TexCoord;\n"
		"};\n"
		"Output test(Input inp) {\n"
		"	Output outp;\n"
		"	outp.Position = ViewProjection * Model * vec4(inp.Position.xy, -inp.Position.z, 1.0);\n"
		"	outp.TexCoord = inp.TexCoord;\n"
		"	return outp;\n"
		"};\n"
		"out Output vs_out;\n"
		"void main() {\n"
		"	Input inp;\n"
		"	inp.Position = Position;\n"
		"	inp.TexCoord = TexCoord;\n"
		"	Output outp = test(inp);\n"
		"	vs_out.Position = outp.Position;\n"
		"	vs_out.TexCoord = outp.TexCoord;\n"
		"	gl_Position = vs_out.Position;\n"
		"}\n";

	const char* src_frag =
		"struct Output {\n"
		"	vec4 Position;\n"
		"	vec2 TexCoord;\n"
		"};\n"
		"in Output vs_out;\n"
		"void main() {\n"
		"	vec4 diffuse = Diffuse * sample_tex(0, vs_out.TexCoord);\n"
		"	vec4 specular = Specular * sample_tex(1, vs_out.TexCoord);\n"
		"	vec4 col = diffuse + specular;\n"
		"	FragColor = vec4(col.xyz, 1.0);\n"
		"	EntityId = Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"#define vec4 float4\n"
		"struct Output {\n"
		"	vec4 Position : SV_Position;\n"
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
		{VEC3F, "Position"},
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
		{A_BFR_VERTEX, 0, vertex, sizeof(vertex), 0, "Input"},
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

	if (shader_create(&model_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_error("Failed to create model shader");
		return NULL;
	}

	return model_renderer;
}

void model_renderer_delete(ModelRenderer* model_renderer) {
	shader_delete(&model_renderer->shader);
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