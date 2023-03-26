#include "gfx_driver.h"

#include "gfx_gl_types.h"

#include "gl/gl_buffer.h"
#include "gl/gl_renderer.h"
#include "gl/gl_shader.h"
#include "gl/gl_texture.h"
#include "gl_ctx/gl_defines.h"

#include "math/mat4.h"
#include "utils/str.h"

typedef struct GLAttachment {
	GLenum format;
	GLenum type;
	GLuint texture;
	GLenum target;
	GLint slot;
} GLAttachment;

struct AFramebuffer {
	int height;

	GLuint fb;
	GLAttachment **attachments;
	uint attachments_count;

	GLuint depth_stencil;

	AShader *shader;
	AMesh *mesh;
};

struct AMesh {
	GLuint va;

	uint vertex_size;
	GLuint vertices;
	GLsizei vertices_count;

	uint instance_size;
	GLuint instances;
	GLsizei instances_count;

	uint index_size;
	GLuint indices;
	GLsizei indices_count;
	GLenum index_type;

	GLenum primitive;
};

struct ARenderer {
	GLError *error;
	LogCallbacks *log;
	int lhc;
};

struct AShader {
	GLuint program;
	GLint textures_location;
	GLuint num_textures;
	GLint *textures;
};

struct ATexture {
	GLuint id;
};

struct AUniformBuffer {
	GLuint buffer;
	GLuint slot;
};

GLenum gl_aprimitive(APrimitive primitive)
{
	switch (primitive) {
	case A_POINTS: return GL_POINTS;
	case A_LINES: return GL_LINES;
	case A_TRIANGLES: return GL_TRIANGLES;
	case A_QUADS: return GL_QUADS;
	}
	return 0;
}

GLenum gl_ashadertype(AShaderType type)
{
	switch (type) {
	case A_FRAGMENT: return GL_FRAGMENT_SHADER;
	case A_VERTEX: return GL_VERTEX_SHADER;
	case A_GEOMETRY: return GL_GEOMETRY_SHADER;
	case A_COMPUTE: return GL_COMPUTE_SHADER;
	}
	return 0;
}

GLint gl_awrap(AWrap wrap)
{
	switch (wrap) {
	case A_REPEAT: return GL_REPEAT;
	case A_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
	}
	return 0;
}

GLenum gl_afactor(AFactor factor)
{
	switch (factor) {
	case A_ZERO: return GL_ZERO;
	case A_ONE: return GL_ONE;
	case A_SRC_COLOR: return GL_SRC_COLOR;
	case A_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
	case A_SRC_ALPHA: return GL_SRC_ALPHA;
	case A_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	case A_DST_ALPHA: return GL_DST_ALPHA;
	case A_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
	case A_DST_COLOR: return GL_DST_COLOR;
	case A_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
	}
	return 0;
}

GLenum gl_adepth_func(ADepthFunc func)
{
	switch (func) {
	case A_DEPTH_NEVER: return GL_NEVER;
	case A_DEPTH_LESS: return GL_LESS;
	case A_DEPTH_EQUAL: return GL_EQUAL;
	case A_DEPTH_LEQUAL: return GL_LEQUAL;
	case A_DEPTH_GRATER: return GL_GREATER;
	case A_DEPTH_NOTEQUAL: return GL_NOTEQUAL;
	case A_DEPTH_GEQUAL: return GL_GEQUAL;
	case A_DEPTH_ALWAYS: return GL_ALWAYS;
	}
	return 0;
}

GLenum gl_atype_type(AType type)
{
	switch (atype_type(type)) {
	case A_B: return GL_BYTE;
	case A_UB: return GL_UNSIGNED_BYTE;
	case A_S: return GL_SHORT;
	case A_US: return GL_UNSIGNED_SHORT;
	case A_I: return GL_INT;
	case A_UI: return GL_UNSIGNED_INT;
	case A_F: return GL_FLOAT;
	case A_D: return GL_DOUBLE;
	}
	return 0;
}

GLint gl_atype_internal_format(AType type)
{
	switch (type) {
	case VEC1B: return GL_R8I;
	case VEC2B: return GL_RG8I;
	case VEC3B: return GL_RGB8I;
	case VEC4B: return GL_RGBA8I;
	case VEC1UB: return GL_R8UI;
	case VEC2UB: return GL_RG8UI;
	case VEC3UB: return GL_RGB8UI;
	case VEC4UB: return GL_RGBA8UI;
	case VEC1S: return GL_R16I;
	case VEC2S: return GL_RG16I;
	case VEC3S: return GL_RGB16I;
	case VEC4S: return GL_RGBA16I;
	case VEC1US: return GL_R16UI;
	case VEC2US: return GL_RG16UI;
	case VEC3US: return GL_RGB16UI;
	case VEC4US: return GL_RGBA16UI;
	case VEC1I: return GL_R32I;
	case VEC2I: return GL_RG32I;
	case VEC3I: return GL_RGB32I;
	case VEC4I: return GL_RGBA32I;
	case VEC1UI: return GL_R32UI;
	case VEC2UI: return GL_RG32UI;
	case VEC3UI: return GL_RGB32UI;
	case VEC4UI: return GL_RGBA32UI;
	case VEC1F: return GL_R32F;
	case VEC2F: return GL_RG32F;
	case VEC3F: return GL_RGB32F;
	case VEC4F: return GL_RGBA32F;
	case VEC1D:
	case VEC2D:
	case VEC3D:
	case VEC4D:
	case MAT4F: return 0;
	}
	return 0;
}

GLenum gl_atype_format(AType type)
{
	switch (type) {
	case VEC1B:
	case VEC1UB:
	case VEC1S:
	case VEC1US:
	case VEC1I: return GL_RED_INTEGER;
	case VEC2B:
	case VEC2UB:
	case VEC2S:
	case VEC2US:
	case VEC2I: return GL_RG_INTEGER;
	case VEC3B:
	case VEC3UB:
	case VEC3S:
	case VEC3US:
	case VEC3I: return GL_RGB_INTEGER;
	case VEC4B:
	case VEC4UB:
	case VEC4S:
	case VEC4US:
	case VEC4I: return GL_RGBA_INTEGER;
	case VEC1F:
	case VEC1D: return GL_RED;
	case VEC2F:
	case VEC2D: return GL_RG;
	case VEC3F:
	case VEC3D: return GL_RGB;
	case VEC4F:
	case VEC4D: return GL_RGBA;
	case MAT4F: return 0;
	}
	return 0;
}

GLAttachment *gl_attachment_create(ARenderer *renderer, AAttachmentDesc desc, GLsizei width, GLsizei height, GLuint index)
{
	GLAttachment *attachment = m_malloc(sizeof(GLAttachment));

	attachment->format  = gl_atype_format(desc.type);
	attachment->type    = gl_atype_type(desc.type);
	attachment->texture = gl_texture_create(renderer->error, gl_awrap(desc.wrap), desc.filter, width, height, gl_atype_internal_format(desc.type), attachment->format,
						attachment->type, NULL);
	if (attachment->texture == 0) {
		log_msg(renderer->log, "Failed to create texture");
		return NULL;
	}

	attachment->target = GL_COLOR_ATTACHMENT0 + index;
	if (gl_fb_attach_texture(renderer->error, attachment->target, attachment->texture) == A_FAIL) {
		log_msg(renderer->log, "Failed to attach texture to framebuffer");
		return NULL;
	}

	return attachment;
}

void gl_attachment_delete(GLAttachment *attachment, ARenderer *renderer)
{
	if (attachment->texture != 0) {
		gl_texture_delete(renderer->error, attachment->texture);
		attachment->texture = 0;
	}
	m_free(attachment, sizeof(GLAttachment));
}

void gl_attachment_bind(ARenderer *renderer, GLAttachment *attachment, GLuint slot)
{
	gl_texture_bind(renderer->error, attachment->texture, slot);
}

void gl_attachment_unbind(ARenderer *renderer, GLAttachment *attachment, GLuint slot)
{
	gl_texture_bind(renderer->error, 0, slot);
}

void gl_attachment_clear(ARenderer *renderer, GLAttachment *attachment, GLint index, const void *value)
{
	gl_cb_clear(renderer->error, attachment->type, index, value);
}

void gl_attachment_read_pixel(ARenderer *renderer, GLAttachment *attachment, int x, int y, void *pixel)
{
	gl_read_pixels(renderer->error, attachment->target, x, y, 1, 1, attachment->format, attachment->type, pixel);
}

static GLuint create_vertex_buffer(ARenderer *renderer, AMesh *mesh, ABufferDesc *desc, ABufferData data)
{
	mesh->vertex_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->vertices	     = gl_vb_create_dynamic(renderer->error, desc->max_count * mesh->vertex_size);
		mesh->vertices_count = 0;
	} else {
		mesh->vertices	     = gl_vb_create_static(renderer->error, data.data, data.size);
		mesh->vertices_count = data.size / mesh->vertex_size;
	}
	return mesh->vertices;
}

static GLuint create_instance_buffer(ARenderer *renderer, AMesh *mesh, ABufferDesc *desc, ABufferData data)
{
	mesh->instance_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->instances	      = gl_vb_create_dynamic(renderer->error, desc->max_count * mesh->instance_size);
		mesh->instances_count = 0;
	} else {
		mesh->instances	      = gl_vb_create_static(renderer->error, data.data, data.size);
		mesh->instances_count = data.size / mesh->instance_size;
	}
	return mesh->instances;
}

static GLuint create_index_buffer(ARenderer *renderer, AMesh *mesh, ABufferDesc *desc, ABufferData data)
{
	mesh->index_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->indices	    = gl_ib_create_dynamic(renderer->error, desc->max_count * mesh->index_size);
		mesh->indices_count = 0;
	} else {
		mesh->indices	    = gl_ib_create_static(renderer->error, data.data, data.size);
		mesh->indices_count = data.size / mesh->index_size;
	}
	mesh->index_type = gl_atype_type(desc->values[0].type);
	return mesh->indices;
}

static bool add_layout(ARenderer *renderer, ABufferDesc *desc, uint *index, GLuint divisor)
{
	uint layout_count = desc->values_size / sizeof(AValue);

	GLuint stride = 0;
	for (uint i = 0; i < layout_count; i++) {
		stride += atype_size(desc->values[i].type);
	}

	byte offset = 0;
	for (uint i = 0; i < layout_count; i++) {
		for (uint j = 0; j < atype_count(desc->values[i].type, 0); j++) {
			if (gl_vertex_attrib(renderer->error, *index, atype_components(desc->values[i].type, 0), gl_atype_type(desc->values[i].type), stride,
					     (void *)(offset), divisor) == A_FAIL) {
				return A_FAIL;
			}
			offset += atype_size(desc->values[i].type == MAT4F ? VEC4F : desc->values[i].type);
			(*index)++;
		}
	}

	return A_SUCCESS;
}

static AMesh *mesh_create(ARenderer *renderer, AShader *shader, AShaderDesc desc, AMeshData data, APrimitive primitive)
{
	AMesh *mesh = m_malloc(sizeof(AMesh));

	ABufferDesc *vertices_desc  = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN0);
	ABufferDesc *instances_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN1);
	ABufferDesc *indices_desc   = ashaderdesc_get_bufferdesc(desc, A_BFR_INDEX);

	mesh->va   = gl_va_create(renderer->error);
	uint index = 0;
	if (vertices_desc != NULL) {
		if (create_vertex_buffer(renderer, mesh, vertices_desc, data.vertices) == 0) {
			log_msg(renderer->log, "Failed to create vertex buffer");
			return NULL;
		}
		if (add_layout(renderer, vertices_desc, &index, 0) == A_FAIL) {
			log_msg(renderer->log, "Failed to add vertex layout");
			return NULL;
		}
	} else {
		mesh->vertices	     = 0;
		mesh->vertices_count = 0;
	}

	if (instances_desc != NULL) {
		if (create_instance_buffer(renderer, mesh, instances_desc, data.instances) == 0) {
			log_msg(renderer->log, "Failed to create instance buffer");
			return NULL;
		}
		if (add_layout(renderer, instances_desc, &index, 1) == A_FAIL) {
			log_msg(renderer->log, "Failed to add instance layout");
			return NULL;
		}
	} else {
		mesh->instances	      = 0;
		mesh->instances_count = 0;
	}

	if (indices_desc != NULL) {
		if (create_index_buffer(renderer, mesh, indices_desc, data.indices) == 0) {
			log_msg(renderer->log, "Failed to create index buffer");
			return NULL;
		}
	} else {
		mesh->indices	    = 0;
		mesh->indices_count = 0;
	}

	mesh->primitive = gl_aprimitive(primitive);

	return mesh;
}

static void mesh_delete(AMesh *mesh, ARenderer *renderer)
{
	if (mesh->va != 0) {
		gl_va_delete(renderer->error, mesh->va);
		mesh->va = 0;
	}
	if (mesh->vertices != 0) {
		gl_vb_delete(renderer->error, mesh->vertices);
		mesh->vertices = 0;
	}
	if (mesh->instances != 0) {
		gl_vb_delete(renderer->error, mesh->instances);
		mesh->instances = 0;
	}
	if (mesh->indices != 0) {
		gl_ib_delete(renderer->error, mesh->indices);
		mesh->indices = 0;
	}
	m_free(mesh, sizeof(AMesh));
}

static void mesh_set_vertices(AMesh *mesh, ARenderer *renderer, const void *vertices, uint vertices_size)
{
	gl_vb_set_data(renderer->error, mesh->vertices, vertices, vertices_size);
	mesh->vertices_count = vertices_size / mesh->vertex_size;
}

static void mesh_set_instances(AMesh *mesh, ARenderer *renderer, const void *instances, uint instances_size)
{
	gl_vb_set_data(renderer->error, mesh->instances, instances, instances_size);
	mesh->instances_count = instances_size / mesh->instance_size;
}

static void mesh_set_indices(AMesh *mesh, ARenderer *renderer, const void *indices, uint indices_size)
{
	gl_ib_set_data(renderer->error, mesh->indices, indices, indices_size);
	mesh->vertices_count = indices_size / mesh->index_size;
}

static void mesh_draw(AMesh *mesh, ARenderer *renderer, uint indices)
{
	gl_va_bind(renderer->error, mesh->va);
	if (mesh->indices == 0) {
		if (mesh->instances == 0) {
			gl_draw_arrays(renderer->error, mesh->primitive, mesh->vertices_count);
		} else {
			gl_draw_arrays_instanced(renderer->error, mesh->primitive, mesh->vertices_count, mesh->instances_count);
		}
	} else {
		gl_ib_bind(renderer->error, mesh->indices);
		if (mesh->instances == 0) {
			gl_draw_elements(renderer->error, mesh->primitive, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->index_type);
		} else {
			gl_draw_elements_instanced(renderer->error, mesh->primitive, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->index_type,
						   mesh->instances_count);
		}
	}
}

static ARenderer *renderer_create(AContext *context, LogCallbacks *log, int lhc)
{
	ARenderer *renderer = m_malloc(sizeof(ARenderer));
	renderer->error	    = &context->error;
	renderer->log	    = log;
	renderer->lhc	    = lhc;

	gl_cull_face_back(renderer->error);
	gl_blend_func(renderer->error, gl_afactor(A_SRC_ALPHA), gl_afactor(A_ONE_MINUS_SRC_ALPHA));
	gl_depth_func(renderer->error, gl_adepth_func(A_DEPTH_LEQUAL));

	if (lhc == 1) {
		gl_front_face_cw(renderer->error);
		gl_depth_func(renderer->error, gl_adepth_func(A_DEPTH_GEQUAL));
	} else {
		gl_front_face_ccw(renderer->error);
		gl_depth_func(renderer->error, gl_adepth_func(A_DEPTH_LEQUAL));
	}

	gl_unpack_alignment(renderer->error);
	return renderer;
}

static void renderer_delete(ARenderer *renderer)
{
	m_free(renderer, sizeof(ARenderer));
}

static void renderer_depth_stencil_set(ARenderer *renderer, bool depth_enabled, bool stencil_enabled)
{
	if (depth_enabled == 0) {
		gl_depth_test_disable(renderer->error);
	} else {
		gl_depth_test_enable(renderer->error);
	}
}

static void renderer_rasterizer_set(ARenderer *renderer, bool wireframe, bool cull_back, bool ccw)
{
	if (wireframe == 0) {
		gl_polygon_mode_fill(renderer->error);
	} else {
		gl_polygon_mode_line(renderer->error);
	}

	if (cull_back == 0) {
		gl_cull_face_disable(renderer->error);
	} else {
		gl_cull_face_enable(renderer->error);
	}

	if (ccw == 0) {
		gl_front_face_cw(renderer->error);
	} else {
		gl_front_face_ccw(renderer->error);
	}
}

static void renderer_blend_set(ARenderer *renderer, bool enabled)
{
	if (enabled == 0) {
		gl_blend_disable(renderer->error);
	} else {
		gl_blend_enable(renderer->error);
	}
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

static const char *get_type_token(AType type)
{
	switch (type) {
	case VEC1B: return "bool";
	case VEC2B: return "bvec2";
	case VEC3B: return "bvec3";
	case VEC4B: return "bvec4";
	case VEC1UB: return "bool";
	case VEC2UB: return "bvec2";
	case VEC3UB: return "bvec3";
	case VEC4UB: return "bvec4";
	case VEC1S: return "int";
	case VEC2S: return "ivec2";
	case VEC3S: return "ivec3";
	case VEC4S: return "ivec4";
	case VEC1US: return "int";
	case VEC2US: return "ivec2";
	case VEC3US: return "ivec3";
	case VEC4US: return "ivec4";
	case VEC1I: return "int";
	case VEC2I: return "ivec2";
	case VEC3I: return "ivec3";
	case VEC4I: return "ivec4";
	case VEC1UI: return "uint";
	case VEC2UI: return "uvec2";
	case VEC3UI: return "uvec3";
	case VEC4UI: return "uvec4";
	case VEC1F: return "float";
	case VEC2F: return "vec2";
	case VEC3F: return "vec3";
	case VEC4F: return "vec4";
	case VEC1D: return "double";
	case VEC2D: return "dvec2";
	case VEC3D: return "dvec3";
	case VEC4D: return "dvec4";
	case MAT4F: return "mat4";
	}
	return "";
}

static void vs_add_input(AShaderDesc desc, Str *str)
{
	int location = 0;

	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		if (desc.buffers[i].type == A_BFR_VS_IN0 || desc.buffers[i].type == A_BFR_VS_IN1) {
			uint values_count = desc.buffers[i].values_size / sizeof(AValue);
			for (uint j = 0; j < values_count; j++) {
				str_add_cstrf(str, "layout (location = %i) in %s %s;\n", location++, get_type_token(desc.buffers[i].values[j].type),
					      desc.buffers[i].values[j].name);
			}
		}
	}
}

static void vs_add_input_converters(AShaderDesc sdesc, Str *str)
{
	int location = 0;
	int n	     = 0;

	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_IN0 || bdesc.type == A_BFR_VS_IN1) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	g_in%i.%s = %s;\n", n, bdesc.values[i].name, bdesc.values[i].name);
			}
			n++;
		}
	}
}

static void vs_add_output_converters(AShaderDesc sdesc, Str *str)
{
	int location = 0;

	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_OUT) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	vs_out.%s = g_out.%s;\n", bdesc.values[i].name, bdesc.values[i].name);
			}
		}
	}
}

static void vs_add_parameters(AShaderDesc sdesc, Str *str)
{
	int location = 0;
	int n	     = 0;

	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_IN0 || bdesc.type == A_BFR_VS_IN1) {
			if (n > 0) {
				str_add_cstr(str, ", ", 0);
			}
			str_add_cstrf(str, "g_in%i", n);
			n++;
		}
	}
}

static void fs_add_output(AShaderDesc desc, Str *str)
{
	int location = 0;

	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		if (desc.buffers[i].type == A_BFR_PS_OUT) {
			uint values_count = desc.buffers[i].values_size / sizeof(AValue);
			for (uint j = 0; j < values_count; j++) {
				str_add_cstrf(str, "layout (location = %i) out %s %s;\n", location++, get_type_token(desc.buffers[i].values[j].type),
					      desc.buffers[i].values[j].name);
			}
		}
	}
}

static void fs_add_input_converters(AShaderDesc sdesc, Str *str)
{
	int location = 0;

	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_OUT) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	g_in.%s = fs_in.%s;\n", bdesc.values[i].name, bdesc.values[i].name);
			}
		}
	}
}

static void fs_add_output_converters(AShaderDesc sdesc, Str *str)
{
	int location = 0;

	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_PS_OUT) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s = g_out.%s;\n", bdesc.values[i].name, bdesc.values[i].name);
			}
		}
	}
}

static void sg_generate(AShaderDesc desc, Str *vert, Str *frag)
{
	vs_add_input(desc, vert);
	str_add_cstr(vert, "void main() {\n", 0);
	vs_add_input_converters(desc, vert);
	str_add_cstr(vert, "	g_out = vs_main(", 0);
	vs_add_parameters(desc, vert);
	str_add_cstr(vert, ");\n", 0);
	vs_add_output_converters(desc, vert);
	str_add_cstr(vert, "	gl_Position = g_out.SV_Position;\n}\n", 0);
	str_add_char(vert, '\0');
	printf("----------------------Vertex----------------------------\n");
	str_print(vert);

	fs_add_output(desc, frag);
	str_add_cstr(frag, "void main() {\n", 0);
	fs_add_input_converters(desc, frag);
	str_add_cstr(frag, "	g_out = fs_main(g_in);\n", 0);
	fs_add_output_converters(desc, frag);
	str_add_cstr(frag, "}\n", 0);
	str_add_char(frag, '\0');

	printf("----------------------Fragment----------------------------\n");
	str_print(frag);
}

static const char *sg_get_bnf()
{
	return "<program>                ::= '#version 440 core' <new_line> { <vs_in_struct> | <vs_out_struct> | <fs_in_struct> | <fs_out_struct> | <buffer_struct> | <function_definition> | <function_prototype> | <textures_declaration> }1\n"
	       "<textures_declaration>   ::= 'uniform sampler2D ' <identifier> '[' <int> '];' <new_line>\n"
	       "<vs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_in' <int> ';' <new_line>\n"
	       "<vs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_out;' <new_line> 'layout (location = 0) out ' <identifier> ' vs_out;' <new_line>\n"
	       "<fs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_in;' <new_line> 'layout (location = 0) in ' <identifier> ' fs_in;' <new_line>\n"
	       "<fs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_out;' <new_line>\n"
	       "<buffer_struct>          ::= 'layout (std140, binding = ' <int> ') uniform ' <identifier> ' {' <new_line> { <tab> <type> ' ' <identifier> ';' <new_line> } '};' <new_line>\n"
	       "<struct_mem_sem>         ::= <tab> <type> ' ' <identifier> ';' <new_line>\n"
	       "<semantic>               ::= <identifier>\n"
	       "<type>$i                 ::= 'void' | 'bool' | 'bool'  | 'flat int' | 'flat int' | 'flat int' | 'flat uint' | 'float' | 'double' | 'bool'  | 'bvec2' | 'bvec3' | 'bvec4' | 'bool'   | 'bvec2'  | 'bvec3'  | 'bvec2'  | 'flat int' | 'flat ivec2' | 'flat ivec3' | 'flat ivec4' | 'flat int' | 'flat ivec2' | 'flat ivec3' | 'flat ivec4' | 'flat int' | 'flat ivec2' | 'flat ivec3' | 'flat ivec4' | 'flat uint' | 'flat uvec2' | 'flat uvec3' | 'flat uvec4' | 'float' | 'vec2'  | 'vec3 ' | 'vec4 ' | 'double' | 'dvec2' | 'dvec3' | 'dvec4' | 'mat4'\n"
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
	       "<mul_call>               ::= <lexpression> ' * ' <rexpression> \n"
	       "<lexpression>            ::= <expression>\n"
	       "<rexpression>            ::= <expression>\n"
	       "<sample_call>            ::= 'texture(' <identifier> '[' <int> '], ' <expression> ')'\n"
	       "<discard_call>           ::= 'discard;' <new_line>\n"
	       "<arguments>              ::= <expression> ', ' <arguments> | <expression>\n"
	       "\0";
}

static GLuint create_program(ARenderer *renderer, GLuint vert, GLuint frag)
{
	GLuint program = gl_program_create(renderer->error);
	if (program == 0) {
		return 0;
	}

	if (gl_program_attach_shader(renderer->error, program, vert) == A_FAIL) {
		return 0;
	}
	if (gl_program_attach_shader(renderer->error, program, frag) == A_FAIL) {
		return 0;
	}

	int status;
	if (gl_program_link(renderer->error, program, &status) == A_FAIL) {
		return 0;
	}

	if (!status) {
		int length = 0;
		gl_program_info_length(renderer->error, program, &length);
		char *info = m_malloc(length);
		gl_program_info(renderer->error, program, length, info);
		log_msg(renderer->log, info);
		m_free(info, length);
		return 0;
	}

	return program;
}

static GLuint compile_shader(ARenderer *renderer, GLenum type, const char *source)
{
	int status;
	GLuint shader = gl_shader_create(renderer->error, type, source, &status);
	if (shader == 0) {
		return 0;
	}

	if (!status) {
		int length;
		gl_shader_info_length(renderer->error, shader, &length);
		char *info = m_malloc(length);
		gl_shader_info(renderer->error, shader, length, info);
		log_msg(renderer->log, info);
		m_free(info, length);
		gl_shader_delete(renderer->error, shader);
		return 0;
	}
	return shader;
}

static AShader *shader_create(ARenderer *renderer, const char *src_vert, const char *src_frag, const char *textures, uint num_textures)
{
	AShader *shader = m_malloc(sizeof(AShader));

	shader->num_textures = num_textures;

	GLuint vert = compile_shader(renderer, gl_ashadertype(A_VERTEX), src_vert);
	if (vert == 0) {
		log_msg(renderer->log, "Failed to compile vertex shader");
		return NULL;
	}

	GLuint frag = compile_shader(renderer, gl_ashadertype(A_FRAGMENT), src_frag);
	if (frag == 0) {
		log_msg(renderer->log, "Failed to compile fragment shader");
		return NULL;
	}

	shader->program = create_program(renderer, vert, frag);
	if (shader->program == 0) {
		log_msg(renderer->log, "Failed to create shader program");
		return NULL;
	}

	gl_shader_delete(renderer->error, vert);
	gl_shader_delete(renderer->error, frag);

	if (num_textures > 0) {
		shader->textures = m_malloc(num_textures * sizeof(GLint));
		for (GLuint i = 0; i < num_textures; i++) {
			shader->textures[i] = i;
		}
		shader->textures_location = gl_program_get_uniform_location(renderer->error, shader->program, textures);
		if (shader->textures_location == -1) {
			log_msg(renderer->log, "Failed to get textures location");
			return NULL;
		}
	} else {
		shader->textures_location = -1;
	}
	return shader;
}

static void shader_delete(AShader *shader, ARenderer *renderer)
{
	if (shader->num_textures > 0) {
		m_free(shader->textures, shader->num_textures * sizeof(GLint));
	}
	if (shader->program != 0) {
		gl_program_delete(renderer->error, shader->program);
		shader->program = 0;
	}
	m_free(shader, sizeof(AShader));
}

static void shader_bind(AShader *shader, ARenderer *renderer)
{
	gl_program_use(renderer->error, shader->program);
	if (shader->num_textures > 0) {
		gl_uniform_vec1i(renderer->error, shader->textures_location, shader->num_textures, shader->textures);
	}
}

static ATexture *texture_create(ARenderer *renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void *data)
{
	ATexture *texture = m_malloc(sizeof(ATexture));

	GLint internal_format = 0;
	GLenum format	      = 0;
	GLenum type	      = GL_UNSIGNED_BYTE;
	if (channels == 4) {
		internal_format = GL_RGBA8;
		format		= GL_RGBA;
	} else if (channels == 3) {
		internal_format = GL_RGB8;
		format		= GL_RGB;
	} else if (channels == 2) {
		internal_format = GL_RG8;
		format		= GL_RG;
	} else if (channels == 1) {
		internal_format = GL_R8;
		format		= GL_RED;
	}

	texture->id = gl_texture_create(renderer->error, gl_awrap(wrap), filter, width, height, internal_format, format, type, data);
	if (texture->id == 0) {
		log_msg(renderer->log, "Failed to create texture");
		return NULL;
	}
	return texture;
}

static void texture_bind(ATexture *texture, ARenderer *renderer, uint slot)
{
	gl_texture_bind(renderer->error, texture->id, slot);
}

static void texture_delete(ATexture *texture, ARenderer *renderer)
{
	if (texture->id != 0) {
		gl_texture_delete(renderer->error, texture->id);
		texture->id = 0;
	}
	m_free(texture, sizeof(ATexture));
}

static AUniformBuffer *ub_create_static(ARenderer *renderer, uint slot, uint data_size, const void *data)
{
	AUniformBuffer *uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer	       = gl_ub_create_static(renderer->error, data, data_size);
	if (uniform_buffer->buffer == 0) {
		log_msg(renderer->log, "Failed to create uniform buffer");
		return NULL;
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

static AUniformBuffer *ub_create_dynamic(ARenderer *renderer, uint slot, uint data_size)
{
	AUniformBuffer *uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	uniform_buffer->buffer	       = gl_ub_create_dynamic(renderer->error, data_size);
	if (uniform_buffer->buffer == 0) {
		log_msg(renderer->log, "Failed to create uniform buffer");
		return NULL;
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

static void ub_delete(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
	if (uniform_buffer->buffer != 0) {
		gl_ub_delete(renderer->error, uniform_buffer->buffer);
		uniform_buffer->buffer = 0;
	}
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

static void ub_upload(AUniformBuffer *uniform_buffer, ARenderer *renderer, const void *data, uint data_size)
{
	gl_ub_set_data(renderer->error, uniform_buffer->buffer, data, data_size);
}

static void ub_bind_vs(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
	gl_ub_bind_base(renderer->error, uniform_buffer->buffer, uniform_buffer->slot);
}

static void ub_bind_ps(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
	gl_ub_bind_base(renderer->error, uniform_buffer->buffer, uniform_buffer->slot);
}

static GLuint create_depth_stencil_attachment(ARenderer *renderer, GLsizei width, GLsizei height)
{
	GLuint texture = gl_texture_create(renderer->error, 0, 0, width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	if (texture == 0) {
		log_msg(renderer->log, "Failed to create depth stencil texture");
		return 0;
	}
	if (gl_fb_attach_texture(renderer->error, GL_DEPTH_STENCIL_ATTACHMENT, texture) == A_FAIL) {
		log_msg(renderer->log, "Failed to attach texture");
		return 0;
	}
	return texture;
}

static AFramebuffer *fb_create(ARenderer *renderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height)
{
	AFramebuffer *framebuffer = m_malloc(sizeof(AFramebuffer));
	framebuffer->height	  = height;

	framebuffer->fb = gl_fb_create(renderer->error);
	if (framebuffer->fb == 0) {
		log_msg(renderer->log, "Failed to create framebuffer");
		return NULL;
	}

	framebuffer->attachments_count = attachments_size / sizeof(AAttachmentDesc);
	framebuffer->attachments       = m_malloc(framebuffer->attachments_count * sizeof(GLAttachment *));

	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		framebuffer->attachments[i] = gl_attachment_create(renderer, attachments[i], width, height, i);
		if (framebuffer->attachments[i] == NULL) {
			log_msg(renderer->log, "Failed to create attachment");
			return NULL;
		}
	}

	framebuffer->depth_stencil = create_depth_stencil_attachment(renderer, width, height);
	if (framebuffer->depth_stencil == 0) {
		log_msg(renderer->log, "Failed to create depth stencil attachment");
		return NULL;
	}

	if (gl_fb_check_status(renderer->error, framebuffer->fb) == 0) {
		log_msg(renderer->log, "Framebuffer is not complete");
		return NULL;
	}

	const char *src_vert = "#version 330 core\n"
			       "layout (location = 0) in vec3 Position;\n"
			       "layout (location = 1) in vec2 TexCoord;\n"
			       "out vec2 VTexCoord;\n"
			       "void main() {\n"
			       "	gl_Position = vec4(Position.xyz, 1.0);\n"
			       "	VTexCoord = TexCoord;\n"
			       "}\0";

	const char *src_frag = "#version 330 core\n"
			       "layout (location = 0) out vec4 FragColor;\n"
			       "in vec2 VTexCoord;\n"
			       "uniform sampler2D Texture;\n"
			       "void main() {\n"
			       "	FragColor = texture(Texture, VTexCoord);\n"
			       "}\0";

	framebuffer->shader = shader_create(renderer, src_vert, src_frag, "Texture", 1);
	if (framebuffer->shader == NULL) {
		log_msg(renderer->log, "Failed to create shader");
		return NULL;
	}

	AValue vertex[] = {
		{ VEC3F, "Position" },
		{ VEC2F, "TexCoord" },
	};

	AValue index[] = {
		{ VEC1UI, "" },
	};

	AValue output[] = {
		{ VEC4F, "Color" },
	};

	ABufferDesc buffers[] = {
		{ A_BFR_VS_IN0, 0, vertex, sizeof(vertex), 0, "Input" },
		{ A_BFR_INDEX, 0, index, sizeof(index), 0, "" },
		{ A_BFR_PS_OUT, 0, output, sizeof(output), 0, "Output" },
	};

	AShaderDesc shader_desc = {
		.buffers	= buffers,
		.buffers_size	= sizeof(buffers),
		.textures_count = 1,
		.texture_type	= VEC4F,
	};

	// clang-format off
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	uint indices[] = {
		0, 1, 2,
		0, 2, 3,
	};
	// clang-format on

	AMeshData md = {
		.vertices.data = vertices,
		.vertices.size = sizeof(vertices),
		.indices.data  = indices,
		.indices.size  = sizeof(indices),
	};

	framebuffer->mesh = mesh_create(renderer, framebuffer->shader, shader_desc, md, A_TRIANGLES);
	if (framebuffer->mesh == NULL) {
		log_msg(renderer->log, "Failed to create mesh");
		return NULL;
	}

	return framebuffer;
}

static void fb_delete(AFramebuffer *framebuffer, ARenderer *renderer)
{
	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		gl_attachment_delete(framebuffer->attachments[i], renderer);
	}
	m_free(framebuffer->attachments, framebuffer->attachments_count * sizeof(GLAttachment *));

	if (framebuffer->depth_stencil != 0) {
		gl_texture_delete(renderer->error, framebuffer->depth_stencil);
		framebuffer->depth_stencil = 0;
	}

	mesh_delete(framebuffer->mesh, renderer);
	shader_delete(framebuffer->shader, renderer);

	if (framebuffer->fb != 0) {
		gl_fb_delete(renderer->error, framebuffer->fb);
		framebuffer->fb = 0;
	}

	m_free(framebuffer, sizeof(AFramebuffer));
}

static void fb_bind_render_targets(AFramebuffer *framebuffer, ARenderer *renderer, uint *targets, uint targets_size)
{
	GLenum buffers[8]  = { 0 };
	uint targets_count = targets_size / sizeof(uint);
	for (uint i = 0; i < targets_count; i++) {
		buffers[i]				   = framebuffer->attachments[targets[i]]->target;
		framebuffer->attachments[targets[i]]->slot = i;
	}
	gl_draw_buffers(renderer->error, targets_count, buffers);
}

static void fb_unbind_render_targets(AFramebuffer *framebuffer, ARenderer *renderer, uint *targets, uint targets_size)
{
	GLenum buffers[8]  = { 0 };
	uint targets_count = targets_size / sizeof(uint);
	gl_draw_buffers(renderer->error, targets_count, buffers);
}

static void fb_clear_attachment(AFramebuffer *framebuffer, ARenderer *renderer, uint id, const void *value)
{
	gl_attachment_clear(renderer, framebuffer->attachments[id], framebuffer->attachments[id]->slot, value);
}

static void fb_clear_depth_attachment(AFramebuffer *framebuffer, ARenderer *renderer, const void *value)
{
	gl_dsb_clear(renderer->error, renderer->lhc == 1 ? 0.0f : 1.0f, 0);
}

static void fb_read_pixel(AFramebuffer *framebuffer, ARenderer *renderer, uint id, int x, int y, void *pixel)
{
	gl_attachment_read_pixel(renderer, framebuffer->attachments[id], x, framebuffer->height - y, pixel);
}

static void fb_draw(AFramebuffer *framebuffer, ARenderer *renderer, uint id)
{
	gl_fb_bind(renderer->error, 0);

	gl_clear(renderer->error, 0, 0, 0, 1, renderer->lhc == 1 ? 0 : 1);
	shader_bind(framebuffer->shader, renderer);
	gl_attachment_bind(renderer, framebuffer->attachments[id], 0);
	mesh_draw(framebuffer->mesh, renderer, 0xFFFFFFFF);
	gl_attachment_unbind(renderer, framebuffer->attachments[id], 0);
	gl_fb_bind(renderer->error, framebuffer->fb);
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

GFX_DRIVER(GL, driver)
