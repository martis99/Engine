#pragma once
#include "gl_types.h"

GLuint gl_shader_create(GLenum type, const GLchar* source, GLint* status);
void gl_shader_info_length(GLuint shader, GLint* length);
void gl_shader_info(GLuint shader, GLsizei length, GLchar* info);
void gl_shader_delete(GLuint shader);

GLuint gl_program_create();
bool gl_program_attach_shader(GLuint program, GLuint shader);
bool gl_program_link(GLuint program, GLint* status);
void gl_program_info_length(GLuint program, GLint* length);
void gl_program_info(GLuint program, GLsizei length, GLchar* info);
void gl_program_use(GLuint program);
GLint gl_program_get_uniform_location(GLuint program, const GLchar* name);
void gl_program_delete(GLuint program);

void gl_uniform_vec1i(GLint location, GLsizei count, const GLint* value);