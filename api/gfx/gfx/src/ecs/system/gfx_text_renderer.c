#include "gfx_text_renderer.h"

#include "ecs/component/transform.h"
#include "gfx_batch_renderer.h"
#include "assets/gfx_font.h"
#include "assets/gfx_material.h"
#include "assets/gfx_shader.h"

#include "math/maths.h"

#include "gfx_shader_creator.h"

#include <string.h>

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

	const char* src_vert =
		"VSOutput vs_main(VSInput vs_input) {\n"
		"	VSOutput vs_output;\n"
		"	vs_output.SV_Position = mul(vec4f(vs_input.Position, 1.0f), mul(Model, ViewProjection));\n"
		"	vs_output.Color = vs_input.Color;\n"
		"	vs_output.TexCoord = vs_input.TexCoord;\n"
		"	vs_output.TexId = vs_input.TexId;\n"
		"	vs_output.Entity = vs_input.Entity;\n"
		"	return vs_output;\n"
		"}\n"
		"\0";

	const char* src_frag =
		"FSOutput fs_main(VSOutput fs_input) {\n"
		"	FSOutput fs_output;\n"
		"	float alpha = sample_tex(fs_input.TexId, fs_input.TexCoord).r;\n"
		"	if (alpha == 0) {\n"
		"		discard();\n"
		"	}\n"
		"	fs_output.FragColor = fs_input.Color * vec4f(1.0f, 1.0f, 1.0f, alpha);\n"
		"	fs_output.EntityId = fs_input.Entity;\n"
		"	return fs_output;\n"
		"}\n\0";

	AValue vs_in[] = {
		{VEC3F, "Position"},
		{VEC4F, "Color"},
		{VEC2F, "TexCoord"},
		{VEC1I, "TexId"},
		{VEC1I, "Entity"}
	};

	AValue vs_out[] = {
		{VEC4F, "SV_Position"},
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
		{A_BFR_VS_IN0, 0, vs_in, sizeof(vs_in), MAX_VERTICES, "VSInput"},
		{A_BFR_VS_OUT, 0, vs_out, sizeof(vs_out), MAX_VERTICES, "VSOutput"},
		{A_BFR_INDEX, 0, index, sizeof(index), 0, ""},
		{A_BFR_GLOBAL, 0, global, sizeof(global), 0, "Camera"},
		{A_BFR_VS, 1, vs, sizeof(vs), 0, "VSMaterial"},
		{A_BFR_PS_OUT, 0, output, sizeof(output), 0, "FSOutput"}
	};

	AShaderDesc shader_desc = { 0 };
	shader_desc.buffers = buffers;
	shader_desc.buffers_size = sizeof(buffers);
	shader_desc.textures_count = 16;
	shader_desc.texture_type = VEC4F;

	if (gfx_sc_create_shader(&renderer->shader_creator, &text_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_msg(renderer->log, "Failed to create text shader");
		return NULL;
	}

	if (material_create(&text_renderer->material, renderer, &text_renderer->shader) == NULL) {
		log_msg(renderer->log, "Failed to create text material");
		return NULL;
	};

	if (batch_renderer_create(&text_renderer->batch_renderer, renderer, &text_renderer->material) == NULL) {
		log_msg(renderer->log, "Failed to create text batch renderer");
		return NULL;
	}

	return text_renderer;
}

void text_renderer_delete(TextRenderer* text_renderer) {
	material_delete(&text_renderer->material, text_renderer->batch_renderer.renderer);
	shader_delete(&text_renderer->shader, text_renderer->batch_renderer.renderer);
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

void text_renderer_begin(TextRenderer* text_renderer) {
	batch_renderer_begin(&text_renderer->batch_renderer);
}

void text_renderer_render(TextRenderer* text_renderer, int id, Transform* transform, Text* text) {
	add_text(text_renderer, transform, text, id);
}

void text_renderer_end(TextRenderer* text_renderer) {
	batch_renderer_end(&text_renderer->transform, &text_renderer->batch_renderer);
}