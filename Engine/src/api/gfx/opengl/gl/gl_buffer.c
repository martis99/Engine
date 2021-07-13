#include "pch.h"
#ifdef E_OPENGL
#include "gl_buffer.h"

GLuint gl_va_create() {
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	gl_va_bind(vertex_array);
	return vertex_array;
}

static GLint element_get_size(ADataType element) {
	switch (element) {
	case VEC1I: return 1;
	case VEC2I: return 2;
	case VEC3I: return 3;
	case VEC4I: return 4;
	case VEC1F: return 1;
	case VEC2F: return 2;
	case VEC3F: return 3;
	case VEC4F: return 4;
	default: return 0;
	}
	return 0;
}

static void va_layout_add_element(GLuint index, ADataType element, GLsizei stride, GLuint offset) {
	int size = element_get_size(element);
	switch (element) {
	case VEC1I:
	case VEC2I:
	case VEC3I:
	case VEC4I:
		glVertexAttribIPointer(index, size, GL_INT, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
		break;
	case VEC1F:
	case VEC2F:
	case VEC3F:
	case VEC4F:
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
		break;
	default: break;
	}
	glEnableVertexAttribArray(index);
}

void gl_va_layout_create(GLuint vertex_array, GLuint layout_size, ADataType* layout) {
	gl_va_bind(vertex_array);

	GLuint layout_count = layout_size / sizeof(GLuint);
	GLsizei stride = 0;
	for (GLuint i = 0; i < layout_count; i++) {
		stride += element_get_size(layout[i]);
	}

	GLuint offset = 0;
	for (GLuint i = 0; i < layout_count; i++) {
		va_layout_add_element(i, layout[i], stride, offset);
		offset += element_get_size(layout[i]);
	}
}

void gl_va_bind(GLuint vertex_array) {
	glBindVertexArray(vertex_array);
}

void gl_va_draw_arrays(GLuint vertex_array, GLenum mode, GLsizei count) {
	gl_va_bind(vertex_array);
	glDrawArrays(mode, 0, count);
}

void gl_va_draw_elements(GLuint vertex_array, GLuint index_buffer, GLenum mode, GLsizei count) {
	gl_ib_bind(vertex_array, index_buffer);
	glDrawElements(mode, count, GL_UNSIGNED_INT, NULL);
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

GLuint gl_vb_create_dynamic(GLuint vertex_array, GLsizeiptr size) {
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	gl_vb_bind(vertex_array, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	return vertex_buffer;
}

void gl_vb_bind(GLuint vertex_array, GLuint vertex_buffer) {
	gl_va_bind(vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
}

void gl_vb_set_data(GLuint vertex_array, GLuint vertex_buffer, GLsizeiptr size, const void* data) {
	gl_vb_bind(vertex_array, vertex_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
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

GLuint gl_ib_create_dynamic(GLuint vertex_array, GLsizeiptr size) {
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	gl_ib_bind(vertex_array, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	return index_buffer;
}

void gl_ib_bind(GLuint vertex_array, GLuint index_buffer) {
	gl_va_bind(vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
}

void gl_ib_set_data(GLuint vertex_array, GLuint index_buffer, GLsizeiptr size, const void* data) {
	gl_ib_bind(vertex_array, index_buffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

void gl_ib_delete(GLuint index_buffer) {
	glDeleteBuffers(1, &index_buffer);
}
#endif