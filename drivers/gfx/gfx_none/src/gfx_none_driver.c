#include "gfx_driver.h"

#include "gfx_none_types.h"

#include "math/maths.h"

typedef struct ARenderer {
	int lhc;
} ARenderer;

static void *mesh_create(void *vmesh, void *vrenderer, void *vshader, AShaderDesc desc, AMeshData data, APrimitive primitive)
{
	return vmesh;
}

static void mesh_delete(void *vmesh, void *vrenderer)
{
}

static void mesh_set_vertices(void *vmesh, void *vrenderer, const void *vertices, uint vertices_size)
{
}

static void mesh_set_instances(void *vmesh, void *vrenderer, const void *instances, uint instances_size)
{
}

static void mesh_set_indices(void *vmesh, void *vrenderer, const void *indices, uint indices_size)
{
}

static void mesh_draw(void *vmesh, void *vrenderer, uint indices)
{
}

static void *renderer_create(void *vrenderer, void *vcontext, int lhc)
{
	ARenderer *renderer = vrenderer;

	renderer->lhc = lhc;
	return renderer;
}

static void renderer_delete(void *vrenderer)
{
}

static void renderer_depth_stencil_set(void *vrenderer, bool depth_enabled, bool stencil_enabled)
{
}

static void renderer_rasterizer_set(void *vrenderer, bool wireframe, bool cull_back, bool ccw)
{
}

static void renderer_blend_set(void *vrenderer, bool enabled)
{
}

static mat4 renderer_perspective(void *vrenderer, float fovy, float aspect, float zNear, float zFar)
{
	ARenderer *renderer = vrenderer;

	return mat4_perspective1(fovy, aspect, zNear, zFar, renderer->lhc);
}

static mat4 renderer_ortho(void *vrenderer, float left, float right, float bottom, float top, float znear, float zfar)
{
	ARenderer *renderer = vrenderer;

	return mat4_ortho1(left, right, bottom, top, znear, zfar, renderer->lhc);
}

static float renderer_near(void *vrenderer)
{
	return -1;
}

static float renderer_far(void *vrenderer)
{
	return 1;
}

static void *shader_create(void *vshader, void *vrenderer, const char *src_vert, const char *src_frag, const char *textures, uint num_textures)
{
	return vshader;
}

static void shader_delete(void *vshader, void *vrenderer)
{
}

static void shader_bind(void *vshader, void *vrenderer)
{
}

static void shader_bind_uniform_block(void *vshader, const char *name, uint index)
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

static void *texture_create(void *vtexture, void *vrenderer, AWrap wrap, AFilter filter, int width, int height, int channels, void *data)
{
	return vtexture;
}

static void texture_bind(void *texture, void *vrenderer, uint slot)
{
}

static void texture_delete(void *texture, void *vrenderer)
{
}

static void *ub_create_static(void *vuniform_buffer, void *vrenderer, uint slot, uint data_size, const void *data)
{
	return vuniform_buffer;
}

static void *ub_create_dynamic(void *vuniform_buffer, void *vrenderer, uint slot, uint data_size)
{
	return vuniform_buffer;
}

static void ub_delete(void *vuniform_buffer, void *vrenderer)
{
}

static void ub_upload(void *vuniform_buffer, void *vrenderer, const void *data, uint data_size)
{
}

static void ub_bind_vs(void *vuniform_buffer, void *vrenderer)
{
}

static void ub_bind_ps(void *vuniform_buffer, void *vrenderer)
{
}

static void *fb_create(void *vframebuffer, void *vrenderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height)
{
	return vframebuffer;
}

static void fb_delete(void *framebuffer, void *vrenderer)
{
}

static void fb_bind_render_targets(void *framebuffer, void *vrenderer, uint *targets, uint targets_size)
{
}

static void fb_unbind_render_targets(void *framebuffer, void *vrenderer, uint *targets, uint targets_size)
{
}

static void fb_clear_attachment(void *framebuffer, void *vrenderer, uint id, const void *value)
{
}

static void fb_clear_depth_attachment(void *framebuffer, void *vrenderer, const void *value)
{
}

static void fb_read_pixel(void *framebuffer, void *vrenderer, uint id, int x, int y, void *pixel)
{
}

static void fb_draw(void *framebuffer, void *vrenderer, uint id)
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
