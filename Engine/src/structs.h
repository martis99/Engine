#pragma once
#include "api/wnd/acursor.h"
#include "api/wnd/awindow.h"
#include "api/ctx/acontext.h"

#include "api/gfx/arenderer.h"
#include "api/gfx/ashader.h"
#include "api/gfx/aframebuffer.h"
#include "api/gfx/amesh.h"
#include "api/gfx/atexture.h"
#include "api/gfx/auniform_buffer.h"

#include "stb/stb_truetype.h"

typedef struct WindowSettings {
	int width;
	int height;
} WindowSettings;

typedef struct Window {
	AWindow* window;
	WindowSettings settings;
} Window;

typedef struct Cursor {
	ACursor* cursor;
} Cursor;

typedef struct Context {
	AContext* context;
} Context;

typedef struct Mesh {
	AMesh* mesh;
} Mesh;

typedef struct Framebuffer {
	AFramebuffer* framebuffer;
} Framebuffer;

typedef struct Shader {
	AShader* shader;
	AValue* layout;
	uint layout_size;
	AValue* props;
	uint props_size;
} Shader;

typedef struct Renderer {
	int width;
	int height;
	ARenderer* renderer;
	Framebuffer framebuffer;
	Shader shader;
	Mesh mesh;
	bool backface_culling;
	bool wireframe;
} Renderer;

typedef struct Texture {
	ATexture* texture;
	int width;
	int height;
	int channels;
} Texture;

typedef struct Material {
	Shader* shader;
	AMaterial* material;
	Texture** textures;
	uint textures_count;
} Material;

typedef struct Image {
	int width;
	int height;
	int channels;
	bool from_file;
	unsigned char* data;
} Image;

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
	stbtt_fontinfo info;
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

typedef struct UniformBuffer {
	AUniformBuffer* uniform_buffer;
} UniformBuffer;

typedef struct ModelMesh {
	Mesh* mesh;
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

typedef struct Scene Scene;;

typedef struct Stats {
	size_t memory;
	int draw_calls;
} Stats;

typedef struct App {
	Stats stats;
	Cursor cursor;
	Window window;
	Context context;
	Renderer renderer;
	Scene* scene;
} App;

typedef struct Constraint {
	bool enabled;
	Entity entity;
	float pos;
	int distance;
} Constraint;

typedef struct Constraints {
	bool r_h, r_v, r_d;
	Constraint l, r, u, d, f, b;
	int w, h;
} Constraints;

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
	void* vertices;
	uint vertices_count;
	size_t vertex_size;
	Texture** textures;
	uint textures_count;
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
	Material* material;
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