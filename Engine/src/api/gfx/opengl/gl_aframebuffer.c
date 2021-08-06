#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/aframebuffer.h"
#include "api/gfx/ashader.h"
#include "api/gfx/amesh.h"
#include "gl_atypes.h"
#include "gl_attachment.h"
#include "gl/gl_buffer.h"

GLuint create_depth_stencil_attachment(GLsizei width, GLsizei height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	return texture;
}

AFramebuffer* aframebuffer_create(ARenderer* renderer, AAttachmentDesc* attachments, uint attachments_size, int width, int height) {
	AFramebuffer* framebuffer = m_malloc(sizeof(AFramebuffer));
	framebuffer->height = height;

	framebuffer->fb = gl_fb_create();

	framebuffer->attachments_count = attachments_size / sizeof(AAttachmentDesc);
	framebuffer->attachments = m_malloc(framebuffer->attachments_count * sizeof(GLAttachment*));

	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		framebuffer->attachments[i] = gl_attachment_create(attachments[i], width, height, i);
	}

	framebuffer->depth_stencil = create_depth_stencil_attachment(width, height);

	if (gl_fb_check_status(framebuffer->fb) == 0) {
		log_error("Failed to create framebuffer");
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

	AMeshDesc desc = { 0 };
	desc.vertices.enabled = 1;
	desc.vertices.layout = vertex;
	desc.vertices.layout_size = sizeof(vertex);
	desc.vertices.data = vertices;
	desc.vertices.data_size = sizeof(vertices);
	desc.indices.enabled = 1;
	desc.indices.layout = index;
	desc.indices.layout_size = sizeof(index);
	desc.indices.data = indices;
	desc.indices.data_size = sizeof(indices);
	desc.instances.enabled = 0;
	desc.instances.layout = NULL;
	desc.instances.layout_size = 0;
	desc.instances.data = NULL;
	desc.instances.data_size = 0;
	framebuffer->mesh = amesh_create(renderer, framebuffer->shader, desc, A_TRIANGLES);
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

	glDeleteTextures(1, &framebuffer->depth_stencil);

	amesh_delete(framebuffer->mesh);
	ashader_delete(framebuffer->shader);

	gl_fb_delete(framebuffer->fb);
	m_free(framebuffer, sizeof(AFramebuffer));
}

void aframebuffer_set_render_targets(AFramebuffer* framebuffer, ARenderer* renderer, uint* targets, uint targets_size) {
	GLenum buffers[8];
	uint targets_count = targets_size / sizeof(uint);
	for (uint i = 0; i < targets_count; i++) {
		buffers[i] = framebuffer->attachments[targets[i]]->target;
		framebuffer->slots[targets[i]] = i;
	}
	glDrawBuffers(targets_count, buffers);
}

void aframebuffer_clear_attachment(AFramebuffer* framebuffer, ARenderer* renderer, uint id, const void* value) {
	gl_attachment_clear(framebuffer->attachments[id], framebuffer->slots[id], value);
}

void aframebuffer_clear_depth_attachment(AFramebuffer* framebuffer, ARenderer* renderer, const void* value) {
	glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);
}

void aframebuffer_read_pixel(AFramebuffer* framebuffer, ARenderer* renderer, uint id, int x, int y, void* pixel) {
	gl_attachment_read_pixel(framebuffer->attachments[id], x, framebuffer->height - y, pixel);
}

void aframebuffer_draw(AFramebuffer* framebuffer, ARenderer* renderer, uint id) {
	gl_fb_bind(0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glClearDepth(1);

	ashader_bind(framebuffer->shader, renderer);
	gl_attachment_bind(framebuffer->attachments[id], 0);
	amesh_draw(framebuffer->mesh, renderer, 0xFFFFFFFF);
	gl_attachment_unbind(framebuffer->attachments[id], 0);

	gl_fb_bind(framebuffer->fb);
}
#endif