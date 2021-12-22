#pragma once
#include "eng_types.h"
#include "api/gfx/gfx_api_types.h"

typedef struct Mesh {
	AMesh* mesh;
} Mesh;

typedef struct Framebuffer {
	AFramebuffer* framebuffer;
} Framebuffer;

typedef struct Texture {
	ATexture* texture;
	int width;
	int height;
	int channels;
} Texture;

typedef struct Shader {
	AShaderDesc desc;
	AShader* shader;
	Image default_image;
	Texture default_texture;
} Shader;

typedef struct Renderer {
	int width;
	int height;
	ARenderer* renderer;
	Framebuffer framebuffer;
	Shader shader;
	Mesh mesh;
} Renderer;

typedef struct Context {
	AContext* context;
} Context;

typedef struct UniformBuffer {
	ABuffer values;
	AUniformBuffer* buffer;
} UniformBuffer;

typedef struct Material {
	Shader* shader;
	bool vs;
	UniformBuffer vs_buffer;
	bool ps;
	UniformBuffer ps_buffer;
	Texture** textures;
	uint textures_count;
	uint textures_cap;
} Material;

typedef struct FontCharacter {
	unsigned char c;
	vec2i pos;
	vec2i size;
	int advance;
	vec2i offset;
} FontCharacter;

#define FIRST_CHARACTER 32
#define LAST_CHARACTER 127
#define CHARACTERS_COUNT LAST_CHARACTER - FIRST_CHARACTER

typedef struct Font {
	unsigned char* data;
	size_t data_size;
	FontCharacter characters[CHARACTERS_COUNT];
	Texture* texture;
	int ascent;
	int descent;
	int line_height;
} Font;

typedef struct Assets {
	Renderer* renderer;
	Dictionary* shaders;
	Dictionary* meshes;
	Dictionary* materials;
	Dictionary* images;
	Dictionary* textures;
	Dictionary* fonts;
	Dictionary* models;
} Assets;

typedef struct ModelMesh {
	Mesh mesh;
	uint material;
} ModelMesh;

typedef struct ModelNode {
	Array meshes;
	Array nodes;
	mat4 transformation;
} ModelNode;

typedef struct Model {
	ModelNode node;
	Array materials;
	Array images;
	Array textures;
} Model;

typedef struct InstanceComponent {
	Mesh* mesh;
	Material* material;
	mat4* transforms;
	uint transforms_size;
	uint transforms_count;
} InstanceComponent;

typedef struct MeshComponent {
	Mesh* mesh;
	Material* material;
} MeshComponent;

typedef struct Sprite {
	Texture* texture;
	vec4 color;
	int sub;
	vec2i pos;
	vec2i size;
	vec4 borders;
} Sprite;

typedef struct Text {
	const char* text;
	Font* font;
	vec4 color;
} Text;

typedef struct Constraint {
	bool enabled;
	Entity entity;
	float pos;
	int distance;
} Constraint;

typedef struct Constraints {
	vec3 resolved;
	vec3 size;
	Constraint l, r, u, d, f, b;
} Constraints;

typedef struct Transform {
	vec3 position;
	vec3 rotation;
	vec3 scale;
	vec3 scale_pref;
} Transform;

typedef struct BatchRenderer {
	Renderer* renderer;
	Shader* shader;
	Material* material;
	Mesh mesh;
	uint vertex_size;
	void* vertices;
	uint vertices_count;
	uint indices_count;
} BatchRenderer;

typedef struct InstanceRenderer {
	Renderer* renderer;
	Shader shader;
} InstanceRenderer;

typedef struct LineVertex {
	vec3 position;
	vec4 color;
	int entity;
} LineVertex;

typedef struct LineRenderer {
	Renderer* renderer;
	Transform transform;
	Shader shader;
	Material material;
	Mesh mesh;
	LineVertex* vertices;
	uint vertices_count;
} LineRenderer;

typedef struct MeshRenderer {
	Renderer* renderer;
	Shader shader;
} MeshRenderer;

typedef struct ModelRenderer {
	Renderer* renderer;
	Shader shader;
} ModelRenderer;

typedef struct SpriteRenderer {
	Transform transform;
	Shader shader;
	Material material;
	BatchRenderer batch_renderer;
} SpriteRenderer;

typedef struct TextRenderer {
	Transform transform;
	Shader shader;
	Material material;
	BatchRenderer batch_renderer;
} TextRenderer;