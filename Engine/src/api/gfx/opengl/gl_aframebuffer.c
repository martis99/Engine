#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/aframebuffer.h"
#include "api/gfx/ashader.h"
#include "api/gfx/amesh.h"
#include "gl_atypes.h"
#include "gl_attachment.h"
#include "gl/gl_defines.h"
#include "gl/gl_buffer.h"
#include "gl/gl_texture.h"

GLuint create_depth_stencil_attachment(GLsizei width, GLsizei height) {
	GLuint texture = gl_texture_create(0, 0, width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL, 0);
	if (texture == 0) {
		log_error("Failed to create depth stencil texture");
		return 0;
	}
	if (gl_fb_attach_texture(GL_DEPTH_STENCIL_ATTACHMENT, texture) == A_FAIL) {
		log_error("Failed to attach texture");
		return 0;
	}
	return texture;
}

AFramebuffer* aframebuffer_create(ARenderer* renderer, AAttachmentDesc* attachments, uint attachments_size, int width, int height) {
	AFramebuffer* framebuffer = m_malloc(sizeof(AFramebuffer));
	framebuffer->height = height;

	framebuffer->fb = gl_fb_create();
	if (framebuffer->fb == 0) {
		log_error("Failed to create framebuffer");
		return NULL;
	}

	framebuffer->attachments_count = attachments_size / sizeof(AAttachmentDesc);
	framebuffer->attachments = m_malloc(framebuffer->attachments_count * sizeof(GLAttachment*));

	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		framebuffer->attachments[i] = gl_attachment_create(attachments[i], width, height, i);
		if (framebuffer->attachments[i] == NULL) {
			log_error("Failed to create attachment");
			return NULL;
		}
	}

	framebuffer->depth_stencil = create_depth_stencil_attachment(width, height);
	if (framebuffer->depth_stencil == 0) {
		log_error("Failed to create depth stencil attachment");
		return NULL;
	}

	if (gl_fb_check_status(framebuffer->fb) == 0) {
		log_error("Framebuffer is not complete");
		return NULL;
	}

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec2 TexCoord;\n"
		"out vec2 VTexCoord;\n"
		"void main() {\n"
		"	gl_Position = vec4(Position.xyz, 1.0);\n"
		"	VTexCoord = TexCoord;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"in vec2 VTexCoord;\n"
		"uniform sampler2D Texture;\n"
		"void main() {\n"
		"	FragColor = texture(Texture, VTexCoord);\n"
		"}\0";

	framebuffer->shader = ashader_create(renderer, src_vert, src_frag, "Texture", 1);
	if (framebuffer->shader == NULL) {
		log_error("Failed to create shader");
		return NULL;
	}

	AValue vertex[] = {
		{"Position", VEC3F},
		{"TexCoord", VEC2F}
	};

	AValue index[] = { {"", VEC1UI} };

	AValue output[] = {
		{"Color", VEC4F}
	};

	ABufferDesc buffers[] = {
		{A_BFR_VERTEX, "Input", 0, vertex, sizeof(vertex)},
		{A_BFR_INDEX, NULL, 0, index, sizeof(index)},
		{A_BFR_PS_OUT, "Output", 0, output, sizeof(output)}
	};

	AShaderDesc shader_desc = { 0 };
	shader_desc.buffers = buffers;
	shader_desc.buffers_size = sizeof(buffers);
	shader_desc.textures_count = 1;
	shader_desc.texture_type = VEC4F;

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	uint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	AMeshData md = { 0 };
	md.vertices.data = vertices;
	md.vertices.size = sizeof(vertices);
	md.indices.data = indices;
	md.indices.size = sizeof(indices);

	framebuffer->mesh = amesh_create(renderer, framebuffer->shader, shader_desc, md, A_TRIANGLES);
	if (framebuffer->mesh == NULL) {
		log_error("Failed to create mesh");
		return NULL;
	}

	return framebuffer;
}

void aframebuffer_delete(AFramebuffer* framebuffer) {
	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		gl_attachment_delete(framebuffer->attachments[i]);
	}
	m_free(framebuffer->attachments, framebuffer->attachments_count * sizeof(GLAttachment*));

	if (framebuffer->depth_stencil != 0) {
		gl_texture_delete(framebuffer->depth_stencil);
		framebuffer->depth_stencil = 0;
	}

	amesh_delete(framebuffer->mesh);
	ashader_delete(framebuffer->shader);

	if (framebuffer->fb != 0) {
		gl_fb_delete(framebuffer->fb);
		framebuffer->fb = 0;
	}

	m_free(framebuffer, sizeof(AFramebuffer));
}

void aframebuffer_set_render_targets(AFramebuffer* framebuffer, ARenderer* renderer, uint* targets, uint targets_size) {
	GLenum buffers[8];
	uint targets_count = targets_size / sizeof(uint);
	for (uint i = 0; i < targets_count; i++) {
		buffers[i] = framebuffer->attachments[targets[i]]->target;
		framebuffer->attachments[targets[i]]->slot = i;
	}
	gl_draw_buffers(targets_count, buffers);
}

void aframebuffer_clear_attachment(AFramebuffer* framebuffer, ARenderer* renderer, uint id, const void* value) {
	gl_attachment_clear(framebuffer->attachments[id], framebuffer->attachments[id]->slot, value);
}

void aframebuffer_clear_depth_attachment(AFramebuffer* framebuffer, ARenderer* renderer, const void* value) {
	gl_dsb_clear(1, 0);
}

void aframebuffer_read_pixel(AFramebuffer* framebuffer, ARenderer* renderer, uint id, int x, int y, void* pixel) {
	gl_attachment_read_pixel(framebuffer->attachments[id], x, framebuffer->height - y, pixel);
}

void aframebuffer_draw(AFramebuffer* framebuffer, ARenderer* renderer, uint id) {
	gl_fb_bind(0);

	gl_clear(0, 0, 0, 1, 1);

	ashader_bind(framebuffer->shader, renderer);
	gl_attachment_bind(framebuffer->attachments[id], 0);
	amesh_draw(framebuffer->mesh, renderer, 0xFFFFFFFF);
	gl_attachment_unbind(framebuffer->attachments[id], 0);

	gl_fb_bind(framebuffer->fb);
}
#endif