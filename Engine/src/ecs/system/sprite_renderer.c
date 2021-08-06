#include "pch.h"
#include "sprite_renderer.h"

#include "assets/shader.h"
#include "assets/material.h"
#include "ecs/system/batch_renderer.h"

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
		"#version 330 core\n"
		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec4 Color;\n"
		"layout (location = 2) in vec2 TexCoord;\n"
		"layout (location = 3) in vec2 TexSize;\n"
		"layout (location = 4) in int  TexId;\n"
		"layout (location = 5) in vec2 SprSize;\n"
		"layout (location = 6) in vec4 SprBorders;\n"
		"layout (location = 7) in int  Entity;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 ViewProjection;\n"
		"};\n"
		"uniform mat4 Model;\n"
		"out vec4     VColor;\n"
		"out vec2     VTexCoord;\n"
		"out vec2     VTexSize;\n"
		"out flat int VTexId;\n"
		"out vec2     VSprSize;\n"
		"out vec4     VSprBorders;\n"
		"out flat int VEntity;\n"
		"void main() {\n"
		"	gl_Position = ViewProjection * Model * vec4(Position, 1.0);\n"
		"	VColor = Color;\n"
		"	VTexCoord = TexCoord;\n"
		"	VTexSize = TexSize;\n"
		"	VTexId = TexId;\n"
		"	VSprSize = SprSize;\n"
		"	VSprBorders = SprBorders;\n"
		"	VEntity = Entity;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"uniform sampler2D Textures[16];"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int EntityId;\n"
		"in vec4     VColor;\n"
		"in vec2     VTexCoord;\n"
		"in vec2     VTexSize;\n"
		"in flat int VTexId;\n"
		"in vec2     VSprSize;\n"
		"in vec4     VSprBorders;\n"
		"in flat int VEntity;\n"
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
		"	vec2 tex_coord = tex_borders(VTexCoord, VTexSize, VSprSize, VSprBorders);\n"
		"	FragColor = VColor * tex_color(VTexId, tex_coord);\n"
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
		"	float2 tex_size    : TexSize;\n"
		"	int    tex_id      : TexId;\n"
		"	float2 spr_size    : SprSize;\n"
		"	float4 spr_borders : SprBorders;\n"
		"	int    entity      : Entity;\n"
		"};\n"
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
		"	output.pos         = mul(float4(input.pos, 1.0f), mul(Model, ViewProjection));\n"
		"	output.color       = input.color;\n"
		"	output.tex_coord   = input.tex_coord;\n"
		"	output.tex_size    = input.tex_size;\n"
		"	output.tex_id      = input.tex_id;\n"
		"	output.spr_size    = input.spr_size;\n"
		"	output.spr_borders = input.spr_borders;\n"
		"	output.entity      = input.entity;\n"
		"	return output;\n"
		"}\0";

	const char* src_frag =
		"Texture2D Textures[4];\n"
		"SamplerState Samplers[4];\n"
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
		"struct Output {\n"
		"	float4 color : SV_Target0;\n"
		"	int entity : SV_Target1;\n"
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
		"float4 tex_color(int tex_id, float2 tex_coord) {\n"
		"	switch (tex_id) {\n"
		"		case 0: return Textures[0].Sample(Samplers[0], tex_coord);\n"
		"		case 1: return Textures[1].Sample(Samplers[1], tex_coord);\n"
		"		case 2: return Textures[2].Sample(Samplers[2], tex_coord);\n"
		"		case 3: return Textures[3].Sample(Samplers[3], tex_coord);\n"
		"	}\n"
		"	return float4(1, 1, 1, 1);\n"
		"}\n"
		"Output main(Input input) {\n"
		"	Output output;\n"
		"	float2 tex_coord = tex_borders(input.tex_coord, input.tex_size, input.spr_size, input.spr_borders);\n"
		"	output.color = input.color * tex_color(input.tex_id, tex_coord);\n"
		"	output.entity = input.entity;\n"
		"	return output;\n"
		"}\0";
#endif
	AValue vertex[] = {
		{"Position", VEC3F},
		{"Color", VEC4F},
		{"TexCoord", VEC2F},
		{"TexSize", VEC2F},
		{"TexId", VEC1I},
		{"SprSize", VEC2F },
		{"SprBorders", VEC4F},
		{"Entity", VEC1I},
	};

	AValue index[] = { {"", VEC1UI} };

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

	if (shader_create(&sprite_renderer->shader, renderer, src_vert, src_frag, md, props, sizeof(props), "Textures", 16) == NULL) {
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