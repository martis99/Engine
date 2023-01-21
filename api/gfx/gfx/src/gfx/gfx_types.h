#pragma once
#include "api/ctx_api_types.h"
#include "api/gfx_api_types.h"
#include "assets/assets_types.h"

#include "utils/array.h"
#include "utils/bnf.h"
#include "utils/dictionary.h"

#include "ecs/ecs_types.h"

typedef struct ShaderCreator {
	Str vert;
	Str frag;
	Str str_vert;
	Str str_frag;
	Bnf from_bnf;
	Bnf to_bnf;
} ShaderCreator;

typedef struct Mesh {
	AMesh *mesh;
} Mesh;

typedef struct Framebuffer {
	AFramebuffer *framebuffer;
} Framebuffer;

typedef struct Texture {
	ATexture *texture;
	int width;
	int height;
	int channels;
} Texture;

typedef struct Shader {
	AShaderDesc desc;
	AShader *shader;
	Image default_image;
	Texture default_texture;
} Shader;

typedef struct UniformBuffer {
	ABuffer values;
	AUniformBuffer *buffer;
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

typedef struct Font {
	unsigned char *data;
	size_t data_size;
	FontCharacter characters[CHARACTERS_COUNT];
	Texture *texture;
	int ascent;
	int descent;
	int line_height;
} Font;

typedef struct ObjectMesh {
	Str name;
	Mesh mesh;
	unsigned int material;
} ObjectMesh;

typedef struct ModelObject ModelObject;
struct ModelObject {
	Str name;
	mat4 transformation;
	unsigned char type;
	ObjectMesh mesh;
	ModelObject *child;
	ModelObject *next;
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
	Font *font;
	vec4 color;
} Text;

typedef struct LineVertex {
	vec3 position;
	vec4 color;
	int entity;
} LineVertex;

typedef struct GfxDriver {
	AFramebuffer *(*fb_create)(ARenderer *renderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height);
	void (*fb_delete)(AFramebuffer *framebuffer, ARenderer *renderer);
	void (*fb_bind_render_targets)(AFramebuffer *framebuffer, ARenderer *renderer, uint *targets, uint targets_size);
	void (*fb_unbind_render_targets)(AFramebuffer *framebuffer, ARenderer *renderer, uint *targets, uint targets_size);
	void (*fb_clear_attachment)(AFramebuffer *framebuffer, ARenderer *renderer, uint id, const void *value);
	void (*fb_clear_depth_attachment)(AFramebuffer *framebuffer, ARenderer *renderer, const void *value);
	void (*fb_read_pixel)(AFramebuffer *framebuffer, ARenderer *renderer, uint id, int x, int y, void *pixel);
	void (*fb_draw)(AFramebuffer *framebuffer, ARenderer *renderer, uint id);

	AMesh *(*mesh_create)(ARenderer *renderer, AShader *shader, AShaderDesc desc, AMeshData data, APrimitive primitive);
	void (*mesh_delete)(AMesh *mesh, ARenderer *renderer);
	void (*mesh_set_vertices)(AMesh *mesh, ARenderer *renderer, const void *vertices, uint vertices_size);
	void (*mesh_set_instances)(AMesh *mesh, ARenderer *renderer, const void *instances, uint instances_size);
	void (*mesh_set_indices)(AMesh *mesh, ARenderer *renderer, const void *indices, uint indices_size);
	void (*mesh_draw)(AMesh *mesh, ARenderer *renderer, uint indices);

	ARenderer *(*renderer_create)(AContext *context, LogCallbacks *log, int lhc);
	void (*renderer_delete)(ARenderer *renderer);
	void (*renderer_depth_stencil_set)(ARenderer *renderer, bool depth_enabled, bool stencil_enabled);
	void (*renderer_rasterizer_set)(ARenderer *renderer, bool wireframe, bool cull_back, bool ccw);
	void (*renderer_blend_set)(ARenderer *renderer, bool enabled);
	mat4 (*renderer_perspective)(ARenderer *renderer, float fovy, float aspect, float zNear, float zFar);
	mat4 (*renderer_ortho)(ARenderer *renderer, float left, float right, float bottom, float top, float near, float far);
	float (*renderer_near)(ARenderer *renderer);
	float (*renderer_far)(ARenderer *renderer);

	AShader *(*shader_create)(ARenderer *renderer, const char *src_vert, const char *src_frag, const char *textures, uint num_textures);
	void (*shader_delete)(AShader *shader, ARenderer *renderer);
	void (*shader_bind)(AShader *shader, ARenderer *renderer);

	void (*sg_generate)(AShaderDesc desc, Str *vert, Str *frag);
	const char *(*sg_get_bnf)();

	ATexture *(*texture_create)(ARenderer *renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void *data);
	void (*texture_bind)(ATexture *texture, ARenderer *renderer, uint slot);
	void (*texture_delete)(ATexture *texture, ARenderer *renderer);

	AUniformBuffer *(*ub_create_static)(ARenderer *renderer, uint slot, uint data_size, const void *data);
	AUniformBuffer *(*ub_create_dynamic)(ARenderer *renderer, uint slot, uint data_size);
	void (*ub_delete)(AUniformBuffer *uniform_buffer, ARenderer *renderer);
	void (*ub_upload)(AUniformBuffer *uniform_buffer, ARenderer *renderer, const void *data, uint data_size);
	void (*ub_bind_vs)(AUniformBuffer *uniform_buffer, ARenderer *renderer);
	void (*ub_bind_ps)(AUniformBuffer *uniform_buffer, ARenderer *renderer);

} GfxDriver;

typedef struct Renderer {
	int width;
	int height;
	ARenderer *renderer;
	ShaderCreator shader_creator;
	Framebuffer framebuffer;
	Shader shader;
	Mesh mesh;
	LogCallbacks *log;
	GfxDriver *driver;
	int lhc;
	int cull_back;
	int draw_calls;
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
