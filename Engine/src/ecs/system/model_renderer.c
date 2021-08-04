#include "pch.h"
#include "model_renderer.h"
#include "ecs/component/transform.h"
#include "ecs/component/mesh_component.h"

#include "assets/shader.h"
#include "assets/model.h"

struct Test {
	mat4 a;
	vec4 b;
	vec4 c;
	float d;
};

ModelRenderer* model_renderer_create(ModelRenderer* model_renderer, Renderer* renderer) {
	model_renderer->renderer = renderer;

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
		"uniform sampler2D Textures[16];\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int EntityId;\n"
		"in vec2 VTexCoord;\n"
		"uniform vec4 Diffuse;\n"
		"uniform vec4 Specular;\n"
		"uniform int Entity;\n"
		"void main() {\n"
		"	vec4 diffuse = Diffuse * texture(Textures[0], VTexCoord);\n"
		"	vec4 specular = Specular * texture(Textures[1], VTexCoord);\n"
		"	vec4 col = diffuse + specular;\n"
		"	FragColor = vec4(col.xyz, 1.0);\n"
		"	EntityId = Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"cbuffer Camera {\n"
		"	row_major matrix ViewProjection;\n"
		"};\n"
		"cbuffer Object {\n"
		"	row_major matrix Model;\n"
		"	float4           Diffuse;\n"
		"	float4           Specular;\n"
		"	int              Entity;\n"
		"};\n"
		"struct Input {\n"
		"	float3 pos       : Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"};\n"
		"struct Output {\n"
		"	float4 pos       : SV_Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"	float4 diffuse   : Diffuse;\n"
		"	float4 specular  : Specular;\n"
		"	int entity       : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos       = mul(float4(input.pos.x, input.pos.y, -input.pos.z, 1.0f), mul(Model, ViewProjection));\n"
		"	output.tex_coord = input.tex_coord;\n"
		"	output.diffuse   = Diffuse;\n"
		"	output.specular  = Specular;\n"
		"	output.entity    = 0;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"Texture2D Textures[4];\n"
		"SamplerState Samplers[4];\n"
		"struct Input {\n"
		"	float4 pos       : SV_Position;\n"
		"	float2 tex_coord : TexCoord;\n"
		"	float4 diffuse   : Diffuse;\n"
		"	float4 specular  : Specular;\n"
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
		"	float4 diffuse = input.diffuse * tex_color(0, input.tex_coord);\n"
		"	float4 specular = input.specular * tex_color(1, input.tex_coord);\n"
		"	float4 col = diffuse + specular;\n"
		"	return float4(col.x, col.y, col.z, 1.0);\n"
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
		{"Diffuse", VEC4F},
		{"Specular", VEC4F},
		{"Entity", VEC1I}
	};

	if (shader_create(&model_renderer->shader, renderer, src_vert, src_frag, md, props, sizeof(props), "Textures", 4) == NULL) {
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
		model_draw(model, model_renderer->renderer, &model_renderer->shader, mat);
	}
}