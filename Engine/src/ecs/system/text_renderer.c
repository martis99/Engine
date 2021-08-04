#include "pch.h"
#include "text_renderer.h"

#include "ecs/component/transform.h"
#include "ecs/component/constraints.h"
#include "ecs/system/batch_renderer.h"
#include "assets/font.h"
#include "assets/material.h"
#include "assets/shader.h"

typedef struct TextVertex {
	vec3 position;
	vec4 color;
	vec2 tex_coord;
	int tex_index;
	int entity;
} TextVertex;

typedef struct TextVertexData {
	vec4 color;
	int entity;
} TextVertexData;

TextRenderer* text_renderer_create(TextRenderer* text_renderer, Renderer* renderer, Transform transform) {
	text_renderer->transform = transform;

#ifdef GAPI_NONE
	const char* src_vert = "";
	const char* src_frag = "";
#elif GAPI_OPENGL
	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec4 Color;\n"
		"layout (location = 2) in vec2 TexCoord;\n"
		"layout (location = 3) in int  TexId;\n"
		"layout (location = 4) in int  Entity;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 ViewProjection;\n"
		"};\n"
		"uniform mat4 Model;\n"
		"out vec4     VColor;\n"
		"out vec2     VTexCoord;\n"
		"out flat int VTexId;\n"
		"out flat int VEntity;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position, 1.0);\n"
		"	VColor = Color;\n"
		"	VTexCoord = TexCoord;\n"
		"	VTexId = TexId;\n"
		"	VEntity = Entity;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"uniform sampler2D Textures[16];"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int EntityId;\n"
		"in vec4     VColor;\n"
		"in vec2     VTexCoord;\n"
		"in flat int VTexId;\n"
		"in flat int VEntity;\n"
		"vec4 tex_color(int tex_id, vec2 tex_coord) {\n"
		"	switch (tex_id) {\n"
		"		case 0: return texture(Textures[0], tex_coord);\n"
		"		case 1: return texture(Textures[1], tex_coord);\n"
		"		case 2: return texture(Textures[2], tex_coord);\n"
		"		case 3: return texture(Textures[3], tex_coord);\n"
		"	}\n"
		"	return vec4(1, 1, 1, 1);\n"
		"}\n"
		"void main() {\n"
		"	FragColor = VColor * vec4(1.0f, 1.0f, 1.0f, tex_color(VTexId, VTexCoord).r);\n"
		"	EntityId = VEntity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"cbuffer Camera {\n"
		"	row_major matrix ViewProjection;\n"
		"};\n"
		"cbuffer Object {\n"
		"	row_major matrix Model;\n"
		"};\n"
		"struct Input {\n"
		"	float3 pos         : Position;\n"
		"	float4 color       : Color;\n"
		"	float2 tex_coord   : TexCoord;\n"
		"	int    tex_id      : TexId;\n"
		"	int    entity      : Entity;\n"
		"};\n"
		"struct Output {\n"
		"	float4 pos         : SV_Position;\n"
		"	float4 color       : Color;\n"
		"	float2 tex_coord   : TexCoord;\n"
		"	int    tex_id      : TexId;\n"
		"	int    entity      : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos         = mul(float4(input.pos, 1.0f), mul(Model, ViewProjection));\n"
		"	output.color       = input.color;\n"
		"	output.tex_coord   = input.tex_coord;\n"
		"	output.tex_id      = input.tex_id;\n"
		"	output.entity      = input.entity;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"Texture2D<float> Textures[4];\n"
		"SamplerState Samplers[4];\n"
		"struct Input {\n"
		"	float4 pos         : SV_Position;\n"
		"	float4 color       : Color;\n"
		"	float2 tex_coord   : TexCoord;\n"
		"	int    tex_id      : TexId;\n"
		"	int    entity      : Entity;\n"
		"};\n"
		"float tex_color(int tex_id, float2 tex_coord) {\n"
		"	switch (tex_id) {\n"
		"		case 0: return Textures[0].Sample(Samplers[0], tex_coord);\n"
		"		case 1: return Textures[1].Sample(Samplers[1], tex_coord);\n"
		"		case 2: return Textures[2].Sample(Samplers[2], tex_coord);\n"
		"		case 3: return Textures[3].Sample(Samplers[3], tex_coord);\n"
		"	}\n"
		"	return float4(1, 1, 1, 1);\n"
		"}\n"
		"float4 main(Input input) : SV_TARGET {\n"
		"	float alpha = tex_color(input.tex_id, input.tex_coord);\n"
		"	clip(alpha == 0 ? -1 : 1);\n"
		"	return input.color * float4(1.0, 1.0, 1.0, tex_color(input.tex_id, input.tex_coord));\n"
		"}\0";
#endif

	AValue vertex[] = {
		{"Position", VEC3F},
		{"Color", VEC4F},
		{"TexCoord", VEC2F},
		{"TexId", VEC1I},
		{"Entity", VEC1I}
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
		{"Model", MAT4F}
	};

	if (shader_create(&text_renderer->shader, renderer, src_vert, src_frag, md, props, sizeof(props), "u_textures", 16) == NULL) {
		log_error("Failed to create text shader");
		return NULL;
	}

	if (material_create(&text_renderer->material, renderer, &text_renderer->shader) == NULL) {
		log_error("Failed to create text material");
		return NULL;
	};

	if (batch_renderer_create(&text_renderer->batch_renderer, renderer, &text_renderer->material) == NULL) {
		log_error("Failed to create text batch renderer");
		return NULL;
	}

	return text_renderer;
}

void text_renderer_delete(TextRenderer* text_renderer) {
	material_delete(&text_renderer->material);
	shader_delete(&text_renderer->shader);
	batch_renderer_delete(&text_renderer->batch_renderer);
}

static void calculate_preffered(Transform* transform, Text* text, Constraints* constraints) {
	float x = 0;
	float y = 0;

	for (size_t i = 0; i < strlen(text->text); i++) {
		FontCharacter fc = font_get_char(text->font, text->text[i]);

		x += fc.advance;
		if (i + 1 < strlen(text->text)) {
			FontCharacter next = font_get_char(text->font, text->text[i + 1]);
			if (constraints->size.x != -1 && x + next.offset.x + next.size.x > constraints->size.x) {
				x = 0;
				y += text->font->line_height;
				if (text->text[i + 1] == ' ') {
					i++;
				}
			}
		}
	}

	transform->scale_pref.x = x + 1;
	transform->scale_pref.y = y + text->font->line_height;
}

static void add_vertex(void* vvertex, vec3 position, vec2 tex_coord, int tex_index, void* vdata) {
	TextVertex* vertex = vvertex;
	TextVertexData* data = vdata;

	vertex->position = position;
	vertex->color = data->color;
	vertex->tex_coord = tex_coord;
	vertex->tex_index = tex_index;
	vertex->entity = data->entity;
}

static void add_text(TextRenderer* text_renderer, Transform* transform, Text* text, int entity) {
	float x = 0;
	float y = 0;

	for (size_t i = 0; i < strlen(text->text); i++) {
		FontCharacter fc = font_get_char(text->font, text->text[i]);

		float px = transform->position.x + x + fc.offset.x;
		float py = transform->position.y + y + fc.offset.y + text->font->ascent;
		float pz = transform->position.z;
		Transform tr = transform_create((vec3) { px, py, pz }, transform->rotation, vec2i_to_vec3(fc.size, 1.0f));

		TextVertexData data;
		data.color = text->color;
		data.entity = entity;

		batch_renderer_add_sub(&text_renderer->batch_renderer, &tr, text->font->texture, fc.pos, fc.size, &data, add_vertex);

		x += fc.advance;
		if (i + 1 < strlen(text->text)) {
			FontCharacter next = font_get_char(text->font, text->text[i + 1]);
			if (x + next.offset.x + next.size.x > transform->scale.x) {
				x = 0;
				y += text->font->line_height;
				if (y > transform->scale.y) {
					return;
				}
				if (text->text[i + 1] == ' ') {
					i++;
				}
			}
		}
	}
}

void text_renderer_render(TextRenderer* text_renderer, Ecs* ecs) {
	batch_renderer_begin(&text_renderer->batch_renderer);
	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_TEXT);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Text* text = (Text*)ecs_get(ecs, qr->list[i], C_TEXT);
		add_text(text_renderer, transform, text, qr->list[i]);
	}
	batch_renderer_end(&text_renderer->transform, &text_renderer->batch_renderer);
}

void text_renderer_calculate_preffered(Ecs* ecs) {
	QueryResult* qr = ecs_query(ecs, 3, C_TRANSFORM, C_TEXT, C_CONSTRAINTS);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Text* text = (Text*)ecs_get(ecs, qr->list[i], C_TEXT);
		Constraints* constraints = (Constraints*)ecs_get(ecs, qr->list[i], C_CONSTRAINTS);
		calculate_preffered(transform, text, constraints);
	}
}