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

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"layout (location = 1) in vec4 a_color;\n"
		"layout (location = 2) in vec2 a_tex_coord;\n"
		"layout (location = 3) in int a_tex_index;\n"
		"layout (location = 4) in int a_entity;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 u_view_projection;\n"
		"};\n"
		"out vec4 v_color;\n"
		"out vec2 v_tex_coord;\n"
		"out flat int v_tex_index;\n"
		"out flat int v_entity;\n"
		"uniform mat4 u_model;\n"
		"void main() {\n"
		"	gl_Position = u_view_projection * u_model * vec4(a_pos.x, a_pos.y, -a_pos.z, 1.0);\n"
		"	v_color = a_color;\n"
		"	v_tex_coord = a_tex_coord;\n"
		"	v_tex_index = a_tex_index;\n"
		"	v_entity = a_entity;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int color2;\n"
		"in vec4 v_color;\n"
		"in vec2 v_tex_coord;\n"
		"in flat int v_tex_index;\n"
		"in flat int v_entity;\n"
		"uniform vec4 u_color;\n"
		"uniform sampler2D u_textures[16];"
		"void main() {\n"
		"	FragColor = v_color * vec4(1.0f, 1.0f, 1.0f, texture(u_textures[v_tex_index], v_tex_coord).r);\n"
		"	color2 = v_entity;\n"
		"}\0";

	AValue layout[] = {
		{"Position", VEC3F},
		{"Color", VEC4F},
		{"TexCoord", VEC2F},
		{"TexIndex", VEC1I},
		{"Entity", VEC1I}
	};

	AValue props[] = {
		{"u_model", MAT4F},
		{"u_color", VEC4F},
	};

	if (shader_create(&text_renderer->shader, renderer, src_vert, src_frag, layout, sizeof(layout), props, sizeof(props), "u_textures", 16) == NULL) {
		log_error("Failed to create text shader");
		return NULL;
	}
	if (material_create(&text_renderer->material, renderer, &text_renderer->shader) == NULL) {
		log_error("Failed to create text material");
		return NULL;
	};

	if (batch_renderer_create(&text_renderer->batch_renderer, renderer, &text_renderer->material, sizeof(TextVertex)) == NULL) {
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

void text_renderer_submit(TextRenderer* text_renderer) {
	batch_renderer_submit(&text_renderer->batch_renderer);
}

static void calculate_preffered(Transform* transform, Text* text, Constraints* constraints) {
	float x = 0;
	float y = 0;

	for (size_t i = 0; i < strlen(text->text); i++) {
		FontCharacter fc = font_get_char(text->font, text->text[i]);

		x += fc.advance;
		if (i + 1 < strlen(text->text)) {
			FontCharacter next = font_get_char(text->font, text->text[i + 1]);
			if (constraints->w != -1 && x + next.offset.x + next.size.x > constraints->w) {
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
	batch_renderer_clear(&text_renderer->batch_renderer);
	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_TEXT);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Text* text = (Text*)ecs_get(ecs, qr->list[i], C_TEXT);
		add_text(text_renderer, transform, text, qr->list[i]);
	}
	text_renderer_submit(text_renderer);
	batch_renderer_draw(&text_renderer->transform, &text_renderer->batch_renderer);
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