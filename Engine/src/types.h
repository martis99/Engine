#pragma once
#include "api/atypes.h"
#include "stb/stb_truetype.h"

typedef struct CameraSettings {
	float width;
	float height;
	float fov;
	float z_near;
	float z_far;

	float move_speed;
	float rotate_speed;
} CameraSettings;

typedef struct Camera {
	CameraSettings settings;

	vec3 position;
	vec3 rotation;

	vec3 right;
	vec3 up;
	vec3 front;

	mat4 translation_matrix;
	mat4 rotation_matrix;

	mat4 view;
	mat4 projection;
	mat4 view_projection;
} Camera;

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

typedef struct Image {
	int width;
	int height;
	int channels;
	bool from_file;
	unsigned char* data;
} Image;

typedef struct Texture {
	ATexture* texture;
	int width;
	int height;
	int channels;
} Texture;

typedef struct Shader {
	AShader* shader;
	AMeshDesc mesh_desc;
	AValue* props;
	uint props_size;
	Image default_image;
	Texture default_texture;
	uint num_textures;
} Shader;

typedef struct Renderer {
	int width;
	int height;
	ARenderer* renderer;
	Framebuffer framebuffer;
	Shader shader;
	Mesh mesh;
} Renderer;

typedef struct Material {
	Shader* shader;
	AMaterial* material;
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
	vec3 resolved;
	vec3 size;
	Constraint l, r, u, d, f, b;
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