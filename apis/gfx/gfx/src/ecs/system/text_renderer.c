#include "text_renderer.h"

#include "ecs/component/transform.h"
#include "ecs/component/constraints.h"
#include "ecs/system/batch_renderer.h"
#include "assets/font.h"
#include "assets/material.h"
#include "assets/shader.h"

#include "math/maths.h"
#include "ecs/ecs.h"

#define MAX_QUADS 200
#define MAX_VERTICES MAX_QUADS * 4

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
		"out Vertex {\n"
		"	vec4     Color;\n"
		"	vec2     TexCoord;\n"
		"	flat int TexId;\n"
		"	flat int Entity;\n"
		"} vs_out;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position, 1.0);\n"
		"	vs_out.Color = Color;\n"
		"	vs_out.TexCoord = TexCoord;\n"
		"	vs_out.TexId = TexId;\n"
		"	vs_out.Entity = Entity;\n"
		"}\0";

	const char* src_frag =
		"in Vertex {\n"
		"	vec4     Color;\n"
		"	vec2     TexCoord;\n"
		"	flat int TexId;\n"
		"	flat int Entity;\n"
		"} vs_in;\n"
		"void main() {\n"
		"	float alpha = sample_tex(vs_in.TexId, vs_in.TexCoord).r;"
		"	if(alpha == 0) {\n"
		"		discard;\n"
		"	}\n"
		"	FragColor = vs_in.Color * vec4(1.0f, 1.0f, 1.0f, alpha);\n"
		"	EntityId = vs_in.Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"struct Output {\n"
		"	float4 pos         : SV_Position;\n"
		"	float4 color       : Color;\n"
		"	float2 tex_coord   : TexCoord;\n"
		"	int    tex_id      : TexId;\n"
		"	int    entity      : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos         = mul(float4(input.Position, 1.0f), mul(Model, ViewProjection));\n"
		"	output.color       = input.Color;\n"
		"	output.tex_coord   = input.TexCoord;\n"
		"	output.tex_id      = input.TexId;\n"
		"	output.entity      = input.Entity;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"struct Input {\n"
		"	float4 pos         : SV_Position;\n"
		"	float4 color       : Color;\n"
		"	float2 tex_coord   : TexCoord;\n"
		"	int    tex_id      : TexId;\n"
		"	int    entity      : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	float alpha = sample_tex(input.tex_id, input.tex_coord);\n"
		"	clip(alpha == 0 ? -1 : 1);\n"
		"	output.FragColor = input.color * float4(1.0, 1.0, 1.0, alpha);\n"
		"	output.EntityId = input.entity;\n"
		"	return output;\n"
		"}\0";
#endif

	AValue vertex[] = {
		{VEC3F, "Position"},
		{VEC4F, "Color"},
		{VEC2F, "TexCoord"},
		{VEC1I, "TexId"},
		{VEC1I, "Entity"}
	};

	AValue index[] = { {VEC1UI, ""} };

	AValue global[] = {
		{MAT4F, "ViewProjection"}
	};

	AValue vs[] = {
		{MAT4F, "Model"}
	};

	AValue output[] = {
		{VEC4F, "FragColor"},
		{VEC1I, "EntityId"}
	};

	ABufferDesc buffers[] = {
		{A_BFR_VERTEX, 0, vertex, sizeof(vertex), MAX_VERTICES, "Input"},
		{A_BFR_INDEX, 0, index, sizeof(index), 0, ""},
		{A_BFR_GLOBAL, 0, global, sizeof(global), 0, "Camera"},
		{A_BFR_VS, 1, vs, sizeof(vs), 0, "VSMaterial"},
		{A_BFR_PS_OUT, 0, output, sizeof(output), 0, "Output"}
	};

	AShaderDesc shader_desc = { 0 };
	shader_desc.buffers = buffers;
	shader_desc.buffers_size = sizeof(buffers);
	shader_desc.textures_count = 16;
	shader_desc.texture_type = VEC4F;

	if (shader_create(&text_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
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

void text_renderer_calculate_preffered(Ecs* ecs) {
	QueryResult* qr = ecs_query(ecs, 3, C_TRANSFORM, C_TEXT, C_CONSTRAINTS);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Text* text = (Text*)ecs_get(ecs, qr->list[i], C_TEXT);
		Constraints* constraints = (Constraints*)ecs_get(ecs, qr->list[i], C_CONSTRAINTS);
		calculate_preffered(transform, text, constraints);
	}
}