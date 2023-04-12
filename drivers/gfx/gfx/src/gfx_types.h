#pragma once
#include "api/ctx_api_types.h"
#include "api/gfx_api_types.h"
#include "assets/assets_types.h"

#include "utils/array.h"
#include "utils/bnf.h"
#include "utils/dictionary.h"

#include "str.h"

#include "ecs/ecs_types.h"

typedef struct ShaderCreator {
	Str vert;
	Str frag;
	Str str_vert;
	Str str_frag;
	Bnf from_bnf;
	Bnf to_bnf;
} ShaderCreator;

#define MESH_SIZE 128

typedef struct Mesh {
	byte mesh[MESH_SIZE];
} Mesh;

#define TEXTURE_SIZE 64

typedef struct Texture {
	int width;
	int height;
	int channels;
	byte texture[TEXTURE_SIZE];
} Texture;

#define SHADER_SIZE 64

typedef struct Shader {
	AShaderDesc desc;
	Image default_image;
	Texture default_texture;
	byte shader[SHADER_SIZE];
} Shader;

#define FRAMEBUFFER_SIZE (MESH_SIZE + SHADER_SIZE + 64)

typedef struct Framebuffer {
	byte framebuffer[FRAMEBUFFER_SIZE];
} Framebuffer;

#define UNIFORM_BUFFER_SIZE 64

typedef struct UniformBuffer {
	ABuffer values;
	byte buffer[UNIFORM_BUFFER_SIZE];
} UniformBuffer;

typedef struct Material {
	Shader *shader;
	bool vs;
	UniformBuffer vs_buffer;
	bool ps;
	UniformBuffer ps_buffer;
	Texture **textures;
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

#define FIRST_CHARACTER	 32
#define LAST_CHARACTER	 127
#define CHARACTERS_COUNT LAST_CHARACTER - FIRST_CHARACTER

typedef struct EFont {
	unsigned char *data;
	size_t data_size;
	FontCharacter characters[CHARACTERS_COUNT];
	Texture *texture;
	int ascent;
	int descent;
	int line_height;
} EFont;

typedef struct ObjectMesh {
	str_t name;
	Mesh mesh;
	unsigned int material;
} ObjectMesh;

typedef struct ModelObject ModelObject;
struct ModelObject {
	str_t name;
	mat4 transformation;
	unsigned char type;
	ObjectMesh mesh;
	ModelObject *child;
	ModelObject *next;
	void *vertices;
	size_t vertices_size;
};

typedef struct Model {
	int objects_count;
	ModelObject *objects;
	int materials_count;
	Material *materials;
	int images_count;
	Image *images;
	int textures_count;
	Texture *textures;
	void *priv;
} Model;

typedef struct InstanceComponent {
	Mesh *mesh;
	Material *material;
	mat4 *transforms;
	uint transforms_size;
	uint transforms_count;
} InstanceComponent;

typedef struct MeshComponent {
	Mesh *mesh;
	Material *material;
} MeshComponent;

typedef struct Sprite {
	Texture *texture;
	vec4 color;
	int sub;
	vec2i pos;
	vec2i size;
	vec4 borders;
} Sprite;

typedef struct Text {
	const char *text;
	EFont *font;
	vec4 color;
} Text;

typedef struct LineVertex {
	vec3 position;
	vec4 color;
	float entity;
} LineVertex;

typedef struct GfxDriver {
	void *(*fb_create)(void *framebuffer, void *renderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height);
	void (*fb_delete)(void *framebuffer, void *renderer);
	void (*fb_bind_render_targets)(void *framebuffer, void *renderer, uint *targets, uint targets_size);
	void (*fb_unbind_render_targets)(void *framebuffer, void *renderer, uint *targets, uint targets_size);
	void (*fb_clear_attachment)(void *framebuffer, void *renderer, uint id, const void *value);
	void (*fb_clear_depth_attachment)(void *framebuffer, void *renderer, const void *value);
	void (*fb_read_pixel)(void *framebuffer, void *renderer, uint id, int x, int y, void *pixel);
	void (*fb_draw)(void *framebuffer, void *renderer, uint id);

	void *(*mesh_create)(void *mesh, void *renderer, void *shader, AShaderDesc desc, AMeshData data, APrimitive primitive);
	void (*mesh_delete)(void *mesh, void *renderer);
	void (*mesh_set_vertices)(void *mesh, void *renderer, const void *vertices, uint vertices_size);
	void (*mesh_set_instances)(void *mesh, void *renderer, const void *instances, uint instances_size);
	void (*mesh_set_indices)(void *mesh, void *renderer, const void *indices, uint indices_size);
	void (*mesh_draw)(void *mesh, void *renderer, uint indices);

	void *(*renderer_create)(void *renderer, void *context, int lhc);
	void (*renderer_delete)(void *renderer);
	void (*renderer_depth_stencil_set)(void *renderer, bool depth_enabled, bool stencil_enabled);
	void (*renderer_rasterizer_set)(void *renderer, bool wireframe, bool cull_back, bool ccw);
	void (*renderer_blend_set)(void *renderer, bool enabled);
	mat4 (*renderer_perspective)(void *renderer, float fovy, float aspect, float zNear, float zFar);
	mat4 (*renderer_ortho)(void *renderer, float left, float right, float bottom, float top, float near, float far);
	float (*renderer_near)(void *renderer);
	float (*renderer_far)(void *renderer);

	void *(*shader_create)(void *shader, void *renderer, const char *src_vert, const char *src_frag, const char *textures, uint num_textures);
	void (*shader_delete)(void *shader, void *renderer);
	void (*shader_bind)(void *shader, void *renderer);

	void (*sg_generate)(AShaderDesc desc, Str *vert, Str *frag);
	const char *(*sg_get_bnf)();

	void *(*texture_create)(void *texture, void *renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void *data);
	void (*texture_bind)(void *texture, void *renderer, uint slot);
	void (*texture_delete)(void *texture, void *renderer);

	void *(*ub_create_static)(void *uniform_buffer, void *renderer, uint slot, uint data_size, const void *data);
	void *(*ub_create_dynamic)(void *uniform_buffer, void *renderer, uint slot, uint data_size);
	void (*ub_delete)(void *uniform_buffer, void *renderer);
	void (*ub_upload)(void *uniform_buffer, void *renderer, const void *data, uint data_size);
	void (*ub_bind_vs)(void *uniform_buffer, void *renderer);
	void (*ub_bind_ps)(void *uniform_buffer, void *renderer);

} GfxDriver;

#define RENDERER_SIZE 256

typedef struct Renderer {
	int width;
	int height;
	ShaderCreator shader_creator;
	Framebuffer framebuffer;
	Shader shader;
	Mesh mesh;
	GfxDriver *driver;
	int lhc;
	int cull_back;
	int draw_calls;
	byte renderer[RENDERER_SIZE];
} Renderer;

typedef struct BatchRenderer {
	Renderer *renderer;
	Shader *shader;
	Material *material;
	Mesh mesh;
	uint vertex_size;
	void *vertices;
	uint vertices_count;
	uint indices_count;
} BatchRenderer;

typedef struct InstanceRenderer {
	Renderer *renderer;
	Shader shader;
} InstanceRenderer;

typedef struct LineRenderer {
	Renderer *renderer;
	Transform transform;
	Shader shader;
	Material material;
	Mesh mesh;
	LineVertex *vertices;
	uint vertices_count;
} LineRenderer;

typedef struct MeshRenderer {
	Renderer *renderer;
	Shader shader;
} MeshRenderer;

typedef struct ModelRenderer {
	Renderer *renderer;
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

typedef struct Assets {
	Renderer *renderer;
	Dictionary *shaders;
	Dictionary *meshes;
	Dictionary *materials;
	Dictionary *images;
	Dictionary *textures;
	Dictionary *fonts;
	Dictionary *models;
} Assets;
