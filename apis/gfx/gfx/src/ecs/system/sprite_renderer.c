#include "sprite_renderer.h"

#include "assets/shader.h"
#include "assets/material.h"
#include "ecs/system/batch_renderer.h"

#include "math/maths.h"
#include "ecs/ecs.h"

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

#ifdef GAPI_NONE
	const char* src_vert = "";
	const char* src_frag = "";
#elif GAPI_OPENGL
	const char* src_vert =
		"out Vertex {\n"
		"	vec4     Color;\n"
		"	vec2     TexCoord;\n"
		"	vec2     TexSize;\n"
		"	flat int TexId;\n"
		"	vec2     SprSize;\n"
		"	vec4     SprBorders;\n"
		"	flat int Entity;\n"
		"} vs_out;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position, 1.0);\n"
		"	vs_out.Color = Color;\n"
		"	vs_out.TexCoord = TexCoord;\n"
		"	vs_out.TexSize = TexSize;\n"
		"	vs_out.TexId = TexId;\n"
		"	vs_out.SprSize = SprSize;\n"
		"	vs_out.SprBorders = SprBorders;\n"
		"	vs_out.Entity = Entity;\n"
		"}\0";

	const char* src_frag =
		"in Vertex {\n"
		"	vec4     Color;\n"
		"	vec2     TexCoord;\n"
		"	vec2     TexSize;\n"
		"	flat int TexId;\n"
		"	vec2     SprSize;\n"
		"	vec4     SprBorders;\n"
		"	flat int Entity;\n"
		"} vs_in;\n"
		"vec2 tex_borders(vec2 tex_coord, vec2 tex_size, vec2 spr_size, vec4 spr_borders) {\n"
		"	vec2 pixel = tex_coord * spr_size;\n"
		"	float l = spr_borders.x;\n"
		"	float r = spr_size.x - spr_borders.y;\n"
		"	float u = spr_borders.z;\n"
		"	float d = spr_size.y - spr_borders.w;\n"
		"	if(pixel.x < l) {\n"
		"		tex_coord.x = pixel.x / tex_size.x;\n"
		"	} else if(pixel.x > r) {\n"
		"		tex_coord.x = 1 - (spr_size.x - pixel.x) / tex_size.x;\n"
		"	} else {\n"
		"		float offset = spr_borders.x / tex_size.x;\n"
		"		float coords = (pixel.x - spr_borders.x) / tex_size.x;\n"
		"		float borders = spr_borders.x + spr_borders.y;"
		"		float scale = (tex_size.x - borders) / (spr_size.x - borders);\n"
		"		tex_coord.x = offset + coords * scale;\n"
		"	}\n"
		"	if(pixel.y < u) {\n"
		"		tex_coord.y = 1 - pixel.y / tex_size.y;\n"
		"	} else if(pixel.y > d) {\n"
		"		tex_coord.y = (spr_size.y - pixel.y) / tex_size.y;\n"
		"	} else {\n"
		"		float offset = spr_borders.z / tex_size.y;\n"
		"		float coords = (pixel.y - spr_borders.z) / tex_size.y;\n"
		"		float borders = spr_borders.z + spr_borders.w;"
		"		float scale = (tex_size.y - borders) / (spr_size.y - borders);\n"
		"		tex_coord.y = 1.0 - (offset + coords * scale);\n"
		"	}\n"
		"	return tex_coord;\n"
		"}\n"
		"void main() {\n"
		"	vec2 tex_coord = tex_borders(vs_in.TexCoord, vs_in.TexSize, vs_in.SprSize, vs_in.SprBorders);\n"
		"	FragColor = vs_in.Color * sample_tex(vs_in.TexId, tex_coord);\n"
		"	EntityId = vs_in.Entity;\n"
		"}\0";
#elif GAPI_DX11
	const char* src_vert =
		"struct Output {\n"
		"	float4 pos         : SV_Position;\n"
		"	float4 color       : Color;\n"
		"	float2 tex_coord   : TexCoord;\n"
		"	float2 tex_size    : TexSize;\n"
		"	int    tex_id      : TexId;\n"
		"	float2 spr_size    : SprSize;\n"
		"	float4 spr_borders : SprBorders;\n"
		"	int    entity      : Entity;\n"
		"};\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	output.pos         = mul(float4(input.Position, 1.0f), mul(Model, ViewProjection));\n"
		"	output.color       = input.Color;\n"
		"	output.tex_coord   = input.TexCoord;\n"
		"	output.tex_size    = input.TexSize;\n"
		"	output.tex_id      = input.TexId;\n"
		"	output.spr_size    = input.SprSize;\n"
		"	output.spr_borders = input.SprBorders;\n"
		"	output.entity      = input.Entity;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"struct Input {\n"
		"	float4 pos         : SV_Position;\n"
		"	float4 color       : Color;\n"
		"	float2 tex_coord   : TexCoord;\n"
		"	float2 tex_size    : TexSize;\n"
		"	int    tex_id      : TexId;\n"
		"	float2 spr_size    : SprSize;\n"
		"	float4 spr_borders : SprBorders;\n"
		"	int    entity      : Entity;\n"
		"};\n"
		"float2 tex_borders(float2 tex_coord, float2 tex_size, float2 spr_size, float4 spr_borders) {\n"
		"	float2 pixel = tex_coord * spr_size;\n"
		"	float l = spr_borders.x;\n"
		"	float r = spr_size.x - spr_borders.y;\n"
		"	float u = spr_borders.z;\n"
		"	float d = spr_size.y - spr_borders.w;\n"
		"	if(pixel.x < l) {\n"
		"		tex_coord.x = pixel.x / tex_size.x;\n"
		"	} else if(pixel.x > r) {\n"
		"		tex_coord.x = 1 - (spr_size.x - pixel.x) / tex_size.x;\n"
		"	} else {\n"
		"		float offset = spr_borders.x / tex_size.x;\n"
		"		float coords = (pixel.x - spr_borders.x) / tex_size.x;\n"
		"		float borders = spr_borders.x + spr_borders.y;"
		"		float scale = (tex_size.x - borders) / (spr_size.x - borders);\n"
		"		tex_coord.x = offset + coords * scale;\n"
		"	}\n"
		"	if(pixel.y < u) {\n"
		"		tex_coord.y = 1 - pixel.y / tex_size.y;\n"
		"	} else if(pixel.y > d) {\n"
		"		tex_coord.y = (spr_size.y - pixel.y) / tex_size.y;\n"
		"	} else {\n"
		"		float offset = spr_borders.z / tex_size.y;\n"
		"		float coords = (pixel.y - spr_borders.z) / tex_size.y;\n"
		"		float borders = spr_borders.z + spr_borders.w;"
		"		float scale = (tex_size.y - borders) / (spr_size.y - borders);\n"
		"		tex_coord.y = 1.0 - (offset + coords * scale);\n"
		"	}\n"
		"	return tex_coord;\n"
		"}\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	float2 tex_coord = tex_borders(input.tex_coord, input.tex_size, input.spr_size, input.spr_borders);\n"
		"	output.FragColor = input.color * sample_tex(input.tex_id, tex_coord);\n"
		"	output.EntityId = input.entity;\n"
		"	return output;\n"
		"}\0";
#endif
	AValue vertex[] = {
		{VEC3F, "Position"},
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

	if (shader_create(&sprite_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_error("Failed to create sprite shader");
		return NULL;
	}

	if (material_create(&sprite_renderer->material, renderer, &sprite_renderer->shader) == NULL) {
		log_error("Failed to create sprite material");
		return NULL;
	}

	if (batch_renderer_create(&sprite_renderer->batch_renderer, renderer, &sprite_renderer->material) == NULL) {
		log_error("Failed to create sprite batch renderer");
		return NULL;
	}

	return sprite_renderer;
}

void sprite_renderer_delete(SpriteRenderer* sprite_renderer) {
	material_delete(&sprite_renderer->material);
	shader_delete(&sprite_renderer->shader);
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

void sprite_renderer_render(SpriteRenderer* sprite_renderer, Ecs* ecs) {
	batch_renderer_begin(&sprite_renderer->batch_renderer);
	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_SPRITE);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Sprite* sprite = (Sprite*)ecs_get(ecs, qr->list[i], C_SPRITE);
		add_sprite(sprite_renderer, transform, sprite, qr->list[i]);
	}
	batch_renderer_end(&sprite_renderer->transform, &sprite_renderer->batch_renderer);
}