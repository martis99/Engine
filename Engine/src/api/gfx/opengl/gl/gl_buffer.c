#include "pch.h"
#ifdef E_OPENGL
#include "gl_buffer.h"

GLuint gl_va_create() {
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	gl_va_bind(vertex_array);
	return vertex_array;
}

static void va_layout_add_element(GLuint index, GLint size, GLsizei stride, GLuint offset) {
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
	glEnableVertexAttribArray(index);
}

void gl_va_layout_create(GLuint vertex_array, GLuint layout_size, GLuint* layout) {
	gl_va_bind(vertex_array);

	GLuint layout_count = layout_size / sizeof(GLuint);
	GLsizei stride = 0;
	for (GLuint i = 0; i < layout_count; i++) {
		stride += layout[i];
	}

	GLuint offset = 0;
	for (GLuint i = 0; i < layout_count; i++) {
		va_layout_add_element(i, layout[i], stride, offset);
		offset += layout[i];
	}
}

void gl_va_bind(GLuint vertex_array) {
	glBindVertexArray(vertex_array);
}

void gl_va_draw_elements(GLuint vertex_array, GLuint index_buffer, GLsizei count) {
	gl_ib_bind(vertex_array, index_buffer);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);
}

void gl_va_delete(GLuint vertex_array) {
	glDeleteVertexArrays(1, &vertex_array);
}

GLuint gl_vb_create_static(GLuint vertex_array, GLsizeiptr size, const void* data) {
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	gl_vb_bind(vertex_array, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	return vertex_buffer;
}

void gl_vb_bind(GLuint vertex_array, GLuint vertex_buffer) {
	gl_va_bind(vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
}

void gl_vb_delete(GLuint vertex_buffer) {
	glDeleteBuffers(1, &vertex_buffer);
}

GLuint gl_ib_create_static(GLuint vertex_array, GLsizeiptr size, const void* data) {
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	gl_ib_bind(vertex_array, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	return index_buffer;
}

void gl_ib_bind(GLuint vertex_array, GLuint index_buffer) {
	gl_va_bind(vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
}

void gl_ib_delete(GLuint index_buffer) {
	glDeleteBuffers(1, &index_buffer);
}
#endif