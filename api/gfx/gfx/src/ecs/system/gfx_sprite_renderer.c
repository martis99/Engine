#include "gfx_sprite_renderer.h"

#include "assets/gfx_shader.h"
#include "assets/gfx_material.h"
#include "gfx_batch_renderer.h"

#include "math/maths.h"

#include "gfx_shader_creator.h"

#define MAX_QUADS 200
#define MAX_VERTICES MAX_QUADS * 4

typedef struct SpriteVertex {
	vec3 position;
	vec4 color;
	vec2 tex_coord;
	vec2 tex_size;
	int tex_index;
	vec2 spr_size;
	vec4 spr_borders;
	int entity;
} SpriteVertex;

typedef struct SpriteVertexData {
	vec4 color;
	vec3 scale;
	vec4 borders;
	vec2 texsize;
	int entity;
} SpriteVertexData;

SpriteRenderer* sprite_renderer_create(SpriteRenderer* sprite_renderer, Renderer* renderer, Transform transform) {
	sprite_renderer->transform = transform;

	const char* src_vert =
		"VSOutput vs_main(VSInput vs_input) {\n"
		"	VSOutput vs_output;\n"
		"	vs_output.SV_Position = mul(vec4f(vs_input.Position, 1.0f), mul(Model, ViewProjection));\n"
		"	vs_output.Color = vs_input.Color;\n"
		"	vs_output.TexCoord = vs_input.TexCoord;\n"
		"	vs_output.TexSize = vs_input.TexSize;\n"
		"	vs_output.TexId = vs_input.TexId;\n"
		"	vs_output.SprSize = vs_input.SprSize;\n"
		"	vs_output.SprBorders = vs_input.SprBorders;\n"
		"	vs_output.Entity = vs_input.Entity;\n"
		"	return vs_output;\n"
		"}\n"
		"\0";

	const char* src_frag =
		"vec2f tex_borders(vec2f tex_coord, vec2f tex_size, vec2f spr_size, vec4f spr_borders) {\n"
		"	vec2f pixel = tex_coord * spr_size;\n"
		"	float l = spr_borders.x;\n"
		"	float r = spr_size.x - spr_borders.y;\n"
		"	float u = spr_borders.z;\n"
		"	float d = spr_size.y - spr_borders.w;\n"
		"	if (pixel.x < l) {\n"
		"		tex_coord.x = pixel.x / tex_size.x;\n"
		"	} else if (pixel.x > r) {\n"
		"		tex_coord.x = 1 - (spr_size.x - pixel.x) / tex_size.x;\n"
		"	} else {\n"
		"		float offset = spr_borders.x / tex_size.x;\n"
		"		float coords = (pixel.x - spr_borders.x) / tex_size.x;\n"
		"		float borders = spr_borders.x + spr_borders.y;\n"
		"		float scale = (tex_size.x - borders) / (spr_size.x - borders);\n"
		"		tex_coord.x = offset + coords * scale;\n"
		"	}\n"
		"	if (pixel.y < u) {\n"
		"		tex_coord.y = 1 - pixel.y / tex_size.y;\n"
		"	} else if (pixel.y > d) {\n"
		"		tex_coord.y = (spr_size.y - pixel.y) / tex_size.y;\n"
		"	} else {\n"
		"		float offset = spr_borders.z / tex_size.y;\n"
		"		float coords = (pixel.y - spr_borders.z) / tex_size.y;\n"
		"		float borders = spr_borders.z + spr_borders.w;\n"
		"		float scale = (tex_size.y - borders) / (spr_size.y - borders);\n"
		"		tex_coord.y = 1.0f - (offset + coords * scale);\n"
		"	}\n"
		"	return tex_coord;\n"
		"}\n"
		"FSOutput fs_main(VSOutput fs_input) {\n"
		"	FSOutput fs_output;\n"
		"	vec2f tex_coord = tex_borders(fs_input.TexCoord, fs_input.TexSize, fs_input.SprSize, fs_input.SprBorders);\n"
		"	fs_output.FragColor = fs_input.Color * sample_tex(fs_input.TexId, tex_coord);\n"
		"	fs_output.EntityId = fs_input.Entity;\n"
		"	return fs_output;\n"
		"}\n\0";

	AValue vs_in[] = {
		{VEC3F, "Position"},
		{VEC4F, "Color"},
		{VEC2F, "TexCoord"},
		{VEC2F, "TexSize"},
		{VEC1I, "TexId"},
		{VEC2F, "SprSize" },
		{VEC4F, "SprBorders"},
		{VEC1I, "Entity"},
	};

	AValue vs_out[] = {
		{VEC4F, "SV_Position"},
		{VEC4F, "Color"},
		{VEC2F, "TexCoord"},
		{VEC2F, "TexSize"},
		{VEC1I, "TexId"},
		{VEC2F, "SprSize" },
		{VEC4F, "SprBorders"},
		{VEC1I, "Entity"},
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

	if (gfx_sc_create_shader(&renderer->shader_creator, &sprite_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_msg(renderer->log, "Failed to create sprite shader");
		return NULL;
	}

	if (material_create(&sprite_renderer->material, renderer, &sprite_renderer->shader) == NULL) {
		log_msg(renderer->log, "Failed to create sprite material");
		return NULL;
	}

	if (batch_renderer_create(&sprite_renderer->batch_renderer, renderer, &sprite_renderer->material) == NULL) {
		log_msg(renderer->log, "Failed to create sprite batch renderer");
		return NULL;
	}

	return sprite_renderer;
}

void sprite_renderer_delete(SpriteRenderer* sprite_renderer) {
	material_delete(&sprite_renderer->material, sprite_renderer->batch_renderer.renderer);
	shader_delete(&sprite_renderer->shader, sprite_renderer->batch_renderer.renderer);
	batch_renderer_delete(&sprite_renderer->batch_renderer);
}

static void add_vertex(void* vvertex, vec3 position, vec2 tex_coord, int tex_index, void* vdata) {
	SpriteVertex* vertex = vvertex;
	SpriteVertexData* data = vdata;

	vertex->position = position;
	vertex->color = data->color;
	vertex->tex_coord = tex_coord;
	vertex->tex_size = data->texsize;
	vertex->tex_index = tex_index;
	vertex->spr_size = vec3_to_vec2(data->scale);
	vertex->spr_borders = data->borders;
	vertex->entity = data->entity;
}

static void add_sprite(SpriteRenderer* sprite_renderer, Transform* transform, Sprite* sprite, int entity) {
	SpriteVertexData data;
	data.color = sprite->color;
	data.scale = transform->scale;
	data.borders = sprite->borders;
	data.texsize = (vec2){ (float)sprite->texture->width, (float)sprite->texture->height };
	data.entity = entity;
	if (sprite->sub == 0) {
		batch_renderer_add(&sprite_renderer->batch_renderer, transform, sprite->texture, &data, add_vertex);
	} else {
		batch_renderer_add_sub(&sprite_renderer->batch_renderer, transform, sprite->texture, sprite->pos, sprite->size, &data, add_vertex);
	}
}

void sprite_renderer_begin(SpriteRenderer* sprite_renderer) {
	batch_renderer_begin(&sprite_renderer->batch_renderer);
}

void sprite_renderer_render(SpriteRenderer* sprite_renderer, int id, Transform* transform, Sprite* sprite) {
	add_sprite(sprite_renderer, transform, sprite, id);
}

void sprite_renderer_end(SpriteRenderer* sprite_renderer) {
	batch_renderer_end(&sprite_renderer->transform, &sprite_renderer->batch_renderer);
}