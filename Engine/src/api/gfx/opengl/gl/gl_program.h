#pragma once
#ifdef E_OPENGL
#include "gl.h"

GLuint gl_program_create();
void gl_program_attach_shader(GLuint program, GLuint shader);
void gl_program_link(GLuint program, GLint* status);
void gl_program_info_length(GLuint program, GLint* length);
void gl_program_info(GLuint program, GLsizei length, GLchar* info);
void gl_program_use(GLuint program);
GLint gl_program_get_uniform_location(GLuint program, const GLchar* name);
void gl_program_delete(GLuint program);

void gl_uniform_vec1i(GLint location, GLsizei count, const GLint* value);
void gl_uniform_vec2i(GLint location, GLsizei count, const GLint* value);
void gl_uniform_vec3i(GLint location, GLsizei count, const GLint* value);
void gl_uniform_vec4i(GLint location, GLsizei count, const GLint* value);
void gl_uniform_vec1f(GLint location, GLsizei count, const GLfloat* value);
void gl_uniform_vec2f(GLint location, GLsizei count, const GLfloat* value);
void gl_uniform_vec3f(GLint location, GLsizei count, const GLfloat* value);
void gl_uniform_vec4f(GLint location, GLsizei count, const GLfloat* value);
void gl_uniform_mat4f(GLint location, GLsizei count, const GLfloat* value);
#endif