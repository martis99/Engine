#include "gfx_driver.h"

#include "gfx_none_types.h"

#include "math/maths.h"

struct AMaterial {
	byte id;
};

struct AFramebuffer {
	byte id;
};

struct AMesh {
	byte id;
};

struct ARenderer {
	int lhc;
};

struct AShader {
	byte id;
};

struct ATexture {
	byte id;
};

struct AUniform {
	byte id;
};

struct AUniformBuffer {
	byte id;
};

static AMesh *mesh_create(ARenderer *renderer, AShader *shader, AShaderDesc desc, AMeshData data, APrimitive primitive)
{
	AMesh *mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

static void mesh_delete(AMesh *mesh, ARenderer *renderer)
{
	m_free(mesh, sizeof(AMesh));
}

static void mesh_set_vertices(AMesh *mesh, ARenderer *renderer, const void *vertices, uint vertices_size)
{
}

static void mesh_set_instances(AMesh *mesh, ARenderer *renderer, const void *instances, uint instances_size)
{
}

static void mesh_set_indices(AMesh *mesh, ARenderer *renderer, const void *indices, uint indices_size)
{
}

static void mesh_draw(AMesh *mesh, ARenderer *renderer, uint indices)
{
}

static ARenderer *renderer_create(AContext *context, int lhc)
{
	ARenderer *renderer = m_malloc(sizeof(ARenderer));
	renderer->lhc	    = lhc;
	return renderer;
}

static void renderer_delete(ARenderer *renderer)
{
	m_free(renderer, sizeof(ARenderer));
}

static void renderer_depth_stencil_set(ARenderer *renderer, bool depth_enabled, bool stencil_enabled)
{
}

static void renderer_rasterizer_set(ARenderer *renderer, bool wireframe, bool cull_back, bool ccw)
{
}

static void renderer_blend_set(ARenderer *renderer, bool enabled)
{
}

static mat4 renderer_perspective(ARenderer *renderer, float fovy, float aspect, float zNear, float zFar)
{
	return mat4_perspective1(fovy, aspect, zNear, zFar, renderer->lhc);
}

static mat4 renderer_ortho(ARenderer *renderer, float left, float right, float bottom, float top, float znear, float zfar)
{
	return mat4_ortho1(left, right, bottom, top, znear, zfar, renderer->lhc);
}

static float renderer_near(ARenderer *renderer)
{
	return -1;
}

static float renderer_far(ARenderer *renderer)
{
	return 1;
}

static AShader *shader_create(ARenderer *renderer, const char *src_vert, const char *src_frag, const char *textures, uint num_textures)
{
	AShader *shader = m_malloc(sizeof(AShader));
	return shader;
}

static void shader_delete(AShader *shader, ARenderer *renderer)
{
	m_free(shader, sizeof(AShader));
}

static void shader_bind(AShader *shader, ARenderer *renderer)
{
}

static void shader_bind_uniform_block(AShader *shader, const char *name, uint index)
{
}

static void sg_generate(AShaderDesc desc, Str *vert, Str *frag)
{
}

static const char *sg_get_bnf()
{
	return "<program>                ::= { <vs_in_struct> | <vs_out_struct> | <fs_in_struct> | <fs_out_struct> | <buffer_struct> | <function_definition> | <function_prototype> | <textures_declaration> }1\n"
	       "<textures_declaration>   ::= 'textures ' <identifier> '[' <int> '];' <new_line> \n"
	       "<vs_in_struct>           ::= 'vs_in' <int> ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
	       "<vs_out_struct>          ::= 'vs_out' ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
	       "<fs_in_struct>           ::= 'fs_in' ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
	       "<fs_out_struct>          ::= 'fs_out' ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
	       "<buffer_struct>          ::= 'buffer' <int> ' ' <identifier> ' {' <new_line> { <tab> <type> ' ' <identifier> ';' <new_line> } '}' <new_line>\n"
	       "<struct_mem_sem>         ::= <tab> <type> ' ' <identifier> ' : ' <semantic> ';' <new_line> | <tab> <type> ' ' <identifier> ';' <new_line>\n"
	       "<semantic>               ::= <identifier>\n"
	       "<type>$i                 ::= 'void' | 'byte' | 'ubyte' | 'short' | 'ushort' | 'int' | 'uint' | 'float' | 'double' | 'vec1b' | 'vec2b' | 'vec3b' | 'vec4b' | 'vec1ub' | 'vec2ub' | 'vec3ub' | 'vec4ub' | 'vec1s' | 'vec2s' | 'vec3s' | 'vec4s' | 'vec1us' | 'vec2us' | 'vec3us' | 'vec4us' | 'vec1i' | 'vec2i' | 'vec3i' | 'vec4i' | 'vec1ui' | 'vec2ui' | 'vec3ui' | 'vec4ui' | 'vec1f' | 'vec2f' | 'vec3f' | 'vec4f' | 'vec1d' | 'vec2d' | 'vec3d' | 'vec4d' | 'mat4f'\n"
	       "<type_name>              ::= <type> | <identifier>\n"
	       "<function_prototype>     ::= <function_header> ';' <new_line>\n"
	       "<function_definition>    ::= <function_header> ' {' <new_line> { <tab> <compound> } '}' <new_line>\n"
	       "<function_header>        ::= <type_name> ' ' <identifier> '(' [ <parameters> ] ')'\n"
	       "<parameters>             ::= <parameter> ', ' <parameters> | <parameter>\n"
	       "<parameter>              ::= <type_name> ' ' <identifier>\n"
	       "<compound>               ::= <return_statement> | <if_statement> | <switch_statement> | <declaration_expression> | <assignment_expression> | <block> | <function_call>\n"
	       "<return_statement>       ::= 'return ' <expression> ';' <new_line>\n"
	       "<if_statement>           ::= 'if (' <expression> ') ' <block> ' else ' <if_statement> | 'if (' <expression> ') ' <block> ' else ' <compound> <new_line> | 'if (' <expression> ') ' <block> <new_line>\n"
	       "<block>                  ::= '{' <new_line> { <tab> <tab> <compound> } <tab> '}'\n"
	       "<switch_statement>       ::= 'switch (' <value> ') {' <new_line> { <tab> 'case ' <int> ': ' <compound> } <tab> '}' <new_line>\n"
	       "<declaration_expression> ::= <type_name> ' ' <identifier> [ <initialization> ] ';' <new_line>\n"
	       "<initialization>         ::= ' = ' <expression>\n"
	       "<assignment_expression>  ::= <lvalue> ' ' <assignment_operator> ' ' <expression> ';' <new_line>\n"
	       "<expression>             ::= <value> ' ' <expression_operator> ' ' <expression> | <paran_expression> ' ' <expression_operator> ' ' <expression> | <paran_expression> | <value>\n"
	       "<paran_expression>       ::= '(' <expression> ')'\n"
	       "<value>                  ::= <float> 'f' | <int> | <negative> | <lvalue> '++' | <lvalue> '--' | <function_call> '.' <value> | <array_access> '.' <value> | <identifier> '.' <value> | <function_call> | <array_access> | <identifier>\n"
	       "<array_access>           ::= <identifier> '[' <value> ']'\n"
	       "<negative>               ::= '-' <value>\n"
	       "<expression_operator>$v  ::= '+' | '-' | '*' | '/' | '<' | '>' | '==' | '>=' | '<='\n"
	       "<assignment_operator>$v  ::= '=' | '+=' | '-=' | '*=' | '/='\n"
	       "<lvalue>                 ::= <function_call> '.' <lvalue> | <array_access> '.' <lvalue>  | <identifier> '.' <lvalue> | <array_access> | <identifier>\n"
	       "<function_call>          ::= <mul_call> | <sample_call> | <discard_call> | <type> '(' [ <arguments> ] ')' | <identifier> '(' [ <arguments> ] ')'\n"
	       "<mul_call>               ::= 'mul(' <lexpression> ', ' <rexpression> ')'\n"
	       "<lexpression>            ::= <expression>\n"
	       "<rexpression>            ::= <expression>\n"
	       "<sample_call>            ::= 'sample(' <identifier> ', ' <int> ', ' <expression> ')'\n"
	       "<discard_call>           ::= 'discard();' <new_line>\n"
	       "<arguments>              ::= <expression> ', ' <arguments> | <expression>\n"
	       "\0";
}

static ATexture *texture_create(ARenderer *renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void *data)
{
	ATexture *texture = m_malloc(sizeof(ATexture));
	return texture;
}

static void texture_bind(ATexture *texture, ARenderer *renderer, uint slot)
{
}

static void texture_delete(ATexture *texture, ARenderer *renderer)
{
	m_free(texture, sizeof(ATexture));
}

static AUniformBuffer *ub_create_static(ARenderer *renderer, uint slot, uint data_size, const void *data)
{
	AUniformBuffer *uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

static AUniformBuffer *ub_create_dynamic(ARenderer *renderer, uint slot, uint data_size)
{
	AUniformBuffer *uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

static void ub_delete(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

static void ub_upload(AUniformBuffer *uniform_buffer, ARenderer *renderer, const void *data, uint data_size)
{
}

static void ub_bind_vs(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
}

static void ub_bind_ps(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
}

static AFramebuffer *fb_create(ARenderer *renderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height)
{
	AFramebuffer *framebuffer = m_malloc(sizeof(AFramebuffer));
	return framebuffer;
}

static void fb_delete(AFramebuffer *framebuffer, ARenderer *renderer)
{
	m_free(framebuffer, sizeof(AFramebuffer));
}

static void fb_bind_render_targets(AFramebuffer *framebuffer, ARenderer *renderer, uint *targets, uint targets_size)
{
}

static void fb_unbind_render_targets(AFramebuffer *framebuffer, ARenderer *renderer, uint *targets, uint targets_size)
{
}

static void fb_clear_attachment(AFramebuffer *framebuffer, ARenderer *renderer, uint id, const void *value)
{
}

static void fb_clear_depth_attachment(AFramebuffer *framebuffer, ARenderer *renderer, const void *value)
{
}

static void fb_read_pixel(AFramebuffer *framebuffer, ARenderer *renderer, uint id, int x, int y, void *pixel)
{
}

static void fb_draw(AFramebuffer *framebuffer, ARenderer *renderer, uint id)
{
}

static GfxDriver driver = {
	.fb_create		    = fb_create,
	.fb_delete		    = fb_delete,
	.fb_clear_depth_attachment  = fb_clear_depth_attachment,
	.fb_clear_attachment	    = fb_clear_attachment,
	.fb_draw		    = fb_draw,
	.fb_read_pixel		    = fb_read_pixel,
	.fb_bind_render_targets	    = fb_bind_render_targets,
	.fb_unbind_render_targets   = fb_unbind_render_targets,
	.mesh_create		    = mesh_create,
	.mesh_set_indices	    = mesh_set_indices,
	.mesh_delete		    = mesh_delete,
	.mesh_draw		    = mesh_draw,
	.mesh_set_instances	    = mesh_set_instances,
	.mesh_set_vertices	    = mesh_set_vertices,
	.renderer_blend_set	    = renderer_blend_set,
	.renderer_create	    = renderer_create,
	.renderer_delete	    = renderer_delete,
	.renderer_depth_stencil_set = renderer_depth_stencil_set,
	.renderer_far		    = renderer_far,
	.renderer_near		    = renderer_near,
	.renderer_ortho		    = renderer_ortho,
	.renderer_perspective	    = renderer_perspective,
	.renderer_rasterizer_set    = renderer_rasterizer_set,
	.sg_generate		    = sg_generate,
	.sg_get_bnf		    = sg_get_bnf,
	.shader_bind		    = shader_bind,
	.shader_create		    = shader_create,
	.shader_delete		    = shader_delete,
	.texture_bind		    = texture_bind,
	.texture_create		    = texture_create,
	.texture_delete		    = texture_delete,
	.ub_bind_ps		    = ub_bind_ps,
	.ub_bind_vs		    = ub_bind_vs,
	.ub_create_dynamic	    = ub_create_dynamic,
	.ub_create_static	    = ub_create_static,
	.ub_delete		    = ub_delete,
	.ub_upload		    = ub_upload,
};

GFX_DRIVER(NONE, driver)
