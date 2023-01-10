#pragma once
#include "gl_types.h"

GLuint gl_shader_create(GLError *error, GLenum type, const GLchar *source, GLint *status);
void gl_shader_info_length(GLError *error, GLuint shader, GLint *length);
void gl_shader_info(GLError *error, GLuint shader, GLsizei length, GLchar *info);
void gl_shader_delete(GLError *error, GLuint shader);

GLuint gl_program_create(GLError *error);
bool gl_program_attach_shader(GLError *error, GLuint program, GLuint shader);
bool gl_program_link(GLError *error, GLuint program, GLint *status);
void gl_program_info_length(GLError *error, GLuint program, GLint *length);
void gl_program_info(GLError *error, GLuint program, GLsizei length, GLchar *info);
void gl_program_use(GLError *error, GLuint program);
GLint gl_program_get_uniform_location(GLError *error, GLuint program, const GLchar *name);
void gl_program_delete(GLError *error, GLuint program);

void gl_uniform_vec1i(GLError *error, GLint location, GLsizei count, const GLint *value);
