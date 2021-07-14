#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/ashader.h"

#include "gl/gl_program.h"
#include "gl/gl_shader.h"
#include "gl/gl_enums.h"

struct AShader {
	GLuint program;
};

static AShader* create_program(AShader* shader, GLuint vert, GLuint frag) {
	shader->program = gl_program_create();

	gl_program_attach_shader(shader->program, vert);
	gl_program_attach_shader(shader->program, frag);

	int status;
	gl_program_link(shader->program, &status);

	if (!status) {
		int length = 0;
		gl_program_info_length(shader->program, &length);
		char* info = m_malloc(length);
		gl_program_info(shader->program, length, info);
		log_error(info);
		m_free(info, length);
		return NULL;
	}

	return shader;
}

static GLuint compile_shader(GLenum type, const char* source) {
	int status;
	GLuint shader = gl_shader_create(type, source, &status);

	if (!status) {
		int length;
		gl_shader_info_length(shader, &length);
		char* info = m_malloc(length);
		gl_shader_info(shader, length, info);
		log_error(info);
		m_free(info, length);
		gl_shader_delete(shader);
		return 0;
	}
	return shader;
}

AShader* ashader_create(const char* src_vert, const char* src_frag) {
	AShader* shader = m_malloc(sizeof(AShader));

	GLuint vert = compile_shader(gl_ashadertype(A_VERTEX), src_vert);
	if (vert == 0) {
		log_error("Failed to compile vertex shader");
		return NULL;
	}

	GLuint frag = compile_shader(gl_ashadertype(A_FRAGMENT), src_frag);
	if (frag == 0) {
		log_error("Failed to compile fragment shader");
		return NULL;
	}

	if (create_program(shader, vert, frag) == NULL) {
		log_error("Failed to create shader program");
		return NULL;
	}

	gl_shader_delete(vert);
	gl_shader_delete(frag);

	return shader;
}

void ashader_delete(AShader* shader) {
	gl_program_delete(shader->program);
	m_free(shader, sizeof(AShader));
}

void ashader_bind(AShader* shader) {
	gl_program_use(shader->program);
}

void ashader_bind_uniform_block(AShader* shader, const char* name, uint index) {
	gl_shader_bind_uniform_block(shader->program, name, index);
}

#endif