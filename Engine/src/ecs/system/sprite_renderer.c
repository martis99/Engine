#include "pch.h"
#include "sprite_renderer.h"

typedef struct SpriteVertex {
	vec3 position;
	vec4 color;
	vec2 tex_coord;
	int tex_index;
	vec2 size;
	vec4 borders;
	int entity;
} SpriteVertex;

typedef struct SpriteVertexData {
	vec4 color;
	vec3 scale;
	vec4 borders;
	int entity;
} SpriteVertexData;

SpriteRenderer* sprite_renderer_create(SpriteRenderer* sprite_renderer, Assets* assets, Transform transform) {
	sprite_renderer->transform = transform;

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"layout (location = 1) in vec4 a_color;\n"
		"layout (location = 2) in vec2 a_tex_coord;\n"
		"layout (location = 3) in int a_tex_index;\n"
		"layout (location = 4) in vec2 a_size;\n"
		"layout (location = 5) in vec4 a_borders;\n"
		"layout (location = 6) in int a_entity;\n"
		"out vec4 v_color;\n"
		"out vec2 v_tex_coord;\n"
		"out flat int v_tex_index;\n"
		"out vec2 v_size;\n"
		"out vec4 v_borders;\n"
		"out flat int v_entity;\n"
		"uniform mat4 u_model;\n"
		"uniform mat4 u_view_projection;\n"
		"void main() {\n"
		"	gl_Position = u_view_projection * u_model * vec4(a_pos.x, a_pos.y, -a_pos.z, 1.0);\n"
		"	v_color = a_color;\n"
		"	v_tex_coord = a_tex_coord;\n"
		"	v_tex_index = a_tex_index;\n"
		"	v_size = a_size;\n"
		"	v_borders = a_borders;\n"
		"	v_entity = a_entity;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int color2;\n"
		"in vec4 v_color;\n"
		"in vec2 v_tex_coord;\n"
		"in flat int v_tex_index;\n"
		"in vec2 v_size;\n"
		"in vec4 v_borders;\n"
		"in flat int v_entity;\n"
		"uniform vec4 u_color;\n"
		"uniform sampler2D u_textures[16];"
		"void main() {\n"
		"	vec2 texture_size = textureSize(u_textures[v_tex_index], 0);\n"
		"	vec2 tex_coord = v_tex_coord;\n"
		"	vec2 pixel = v_tex_coord * v_size;\n"
		"	float l = v_borders.x;\n"
		"	float r = v_size.x - v_borders.y;\n"
		"	float u = v_borders.z;\n"
		"	float d = v_size.y - v_borders.w;\n"
		"	if(pixel.x < l) {\n"
		"		tex_coord.x = pixel.x / texture_size.x;\n"
		"	} else if(pixel.x > r) {\n"
		"		tex_coord.x = 1 - (v_size.x - pixel.x) / texture_size.x;\n"
		"	} else {\n"
		"		float offset = v_borders.x / texture_size.x;\n"
		"		float coords = (pixel.x - v_borders.x) / texture_size.x;\n"
		"		float borders = v_borders.x + v_borders.y;"
		"		float scale = (texture_size.x - borders) / (v_size.x - borders);\n"
		"		tex_coord.x = offset + coords * scale;\n"
		"	}\n"
		"	if(pixel.y < u) {\n"
		"		tex_coord.y = 1 - pixel.y / texture_size.y;\n"
		"	} else if(pixel.y > d) {\n"
		"		tex_coord.y = (v_size.y - pixel.y) / texture_size.y;\n"
		"	} else {\n"
		"		float offset = v_borders.z / texture_size.y;\n"
		"		float coords = (pixel.y - v_borders.z) / texture_size.y;\n"
		"		float borders = v_borders.z + v_borders.w;"
		"		float scale = (texture_size.y - borders) / (v_size.y - borders);\n"
		"		tex_coord.y = 1.0 - (offset + coords * scale);\n"
		"	}\n"
		"	FragColor = v_color * texture(u_textures[int(v_tex_index)], tex_coord);\n"
		"	color2 = v_entity;\n"
		"}\0";

	Shader* shader = assets_shader_create(assets, "sprite_shader", src_vert, src_frag);
	if (shader == NULL) {
		log_error("Failed to create sprite shader");
		return NULL;
	}
	Material* material = assets_material_create(assets, "sprite_material", shader);

	ADataType layout[] = { VEC3F, VEC4F, VEC2F, VEC1I, VEC2F, VEC4F, VEC1I };
	if (batch_renderer_create(&sprite_renderer->batch_renderer, material, layout, sizeof(layout), sizeof(SpriteVertex)) == NULL) {
		log_error("Failed to create sprite batch renderer");
		return NULL;
	}

	return sprite_renderer;
}

void sprite_renderer_delete(SpriteRenderer* sprite_renderer) {
	batch_renderer_delete(&sprite_renderer->batch_renderer);
}


void sprite_renderer_submit(SpriteRenderer* text_renderer) {
	batch_renderer_submit(&text_renderer->batch_renderer);
}

static void add_vertex(void* vvertex, vec3 position, vec2 tex_coord, int tex_index, void* vdata) {
	SpriteVertex* vertex = vvertex;
	SpriteVertexData* data = vdata;

	vertex->position = position;
	vertex->color = data->color;
	vertex->tex_coord = tex_coord;
	vertex->tex_index = tex_index;
	vertex->size = vec3_to_vec2(data->scale);
	vertex->borders = data->borders;
	vertex->entity = data->entity;
}

static void add_sprite(SpriteRenderer* sprite_renderer, Transform* transform, Sprite* sprite, int entity) {
	SpriteVertexData data;
	data.color = sprite->color;
	data.scale = transform->scale;
	data.borders = sprite->borders;
	data.entity = entity;
	if (sprite->sub == 0) {
		batch_renderer_add(&sprite_renderer->batch_renderer, transform, sprite->texture, &data, add_vertex);
	} else {
		batch_renderer_add_sub(&sprite_renderer->batch_renderer, transform, sprite->texture, sprite->pos, sprite->size, &data, add_vertex);
	}
}

void sprite_renderer_render(SpriteRenderer* sprite_renderer, Ecs* ecs, mat4* view_projection) {
	batch_renderer_clear(&sprite_renderer->batch_renderer);
	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_SPRITE);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Sprite* sprite = (Sprite*)ecs_get(ecs, qr->list[i], C_SPRITE);
		add_sprite(sprite_renderer, transform, sprite, qr->list[i]);
	}
	sprite_renderer_submit(sprite_renderer);
	batch_renderer_draw(&sprite_renderer->transform, &sprite_renderer->batch_renderer, view_projection);
}