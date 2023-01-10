#include "api/gfx/gfx_api_shader.h"
#include "gl/gl_shader.h"

#include "gfx_gl_types.h"

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

AShader *ashader_create(ARenderer *renderer, const char *src_vert, const char *src_frag, const char *textures, uint num_textures)
{
	AShader *shader = m_malloc(sizeof(AShader));

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
		shader->num_textures = num_textures;
	}
	return shader;
}

void ashader_delete(AShader *shader, ARenderer *renderer)
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

void ashader_bind(AShader *shader, ARenderer *renderer)
{
	gl_program_use(renderer->error, shader->program);
	if (shader->num_textures > 0) {
		gl_uniform_vec1i(renderer->error, shader->textures_location, shader->num_textures, shader->textures);
	}
}
