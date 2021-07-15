#include "pch.h"
#ifdef E_OPENGL
#include "gl_buffer.h"

static void va_bind(GLuint va) {
	glBindVertexArray(va);
}

static void va_unbind(GLuint va) {
	glBindVertexArray(0);
}

static void vb_bind(GLuint vb) {
	glBindBuffer(GL_ARRAY_BUFFER, vb);
}

static void vb_unbind(GLuint vb) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void ib_bind(GLuint ib) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

static void ib_unbind(GLuint ib) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint gl_va_create() {
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
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
	case MAT4F: return 16;
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
		glEnableVertexAttribArray(index);
		glVertexAttribIPointer(index, size, GL_INT, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
		break;
	case VEC1F:
	case VEC2F:
	case VEC3F:
	case VEC4F:
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
		break;
	case MAT4F:
		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(index + i);
			glVertexAttribPointer(index + i, 4, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(offset + i * sizeof(vec4)));
			glVertexAttribDivisor(index + i, 1);
		}
		break;
	default: break;
	}
}

static void va_add_layout(ADataType* layout, GLuint layout_size, GLuint* index) {
	GLuint layout_count = layout_size / sizeof(GLuint);
	GLsizei stride = 0;
	for (GLuint i = 0; i < layout_count; i++) {
		stride += element_get_size(layout[i]);
	}

	GLuint offset = 0;
	for (GLuint i = 0; i < layout_count; i++) {
		va_layout_add_element(*index + i, layout[i], stride, offset);
		offset += element_get_size(layout[i]);
	}

	*index += layout_count;
}

void gl_va_draw_arrays(GLuint va, GLenum mode, GLsizei count) {
	va_bind(va);
	glDrawArrays(mode, 0, count);
}

void gl_va_draw_arrays_instanced(GLuint va, GLenum mode, GLsizei count, GLsizei instancecount) {
	va_bind(va);
	glDrawArraysInstanced(mode, 0, count, instancecount);
}

void gl_va_draw_elements(GLuint va, GLuint ib, GLenum mode, GLsizei count) {
	va_bind(va);
	ib_bind(ib);
	glDrawElements(mode, count, GL_UNSIGNED_INT, NULL);
}

void gl_va_draw_elements_instanced(GLuint va, GLuint ib, GLenum mode, GLsizei count, GLsizei instancecount) {
	va_bind(va);
	ib_bind(ib);
	glDrawElementsInstanced(mode, count, GL_UNSIGNED_INT, NULL, instancecount);
}

void gl_va_delete(GLuint va) {
	glDeleteVertexArrays(1, &va);
}

GLuint gl_vb_create() {
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	return vertex_buffer;
}

void gl_vb_init_static(GLuint vb, GLuint va, const void* vertices, GLsizeiptr vertices_size, ADataType* layout, GLuint layout_size, GLuint* index) {
	vb_bind(vb);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
	va_bind(va);
	va_add_layout(layout, layout_size, index);
	va_unbind(va);
	vb_unbind(vb);
}

void gl_vb_init_dynamic(GLuint vb, GLuint va, GLsizeiptr vertices_size, ADataType* layout, GLuint layout_size, GLuint* index) {
	vb_bind(vb);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, NULL, GL_DYNAMIC_DRAW);
	va_bind(va);
	va_add_layout(layout, layout_size, index);
	va_unbind(va);
	vb_unbind(vb);
}

void gl_vb_set_data(GLuint vb, const void* vertices, GLsizeiptr vertices_size) {
	vb_bind(vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, vertices);
}

void gl_vb_delete(GLuint vb) {
	glDeleteBuffers(1, &vb);
}

GLuint gl_ib_create() {
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	return index_buffer;
}

void gl_ib_init_static(GLuint ib, const void* indices, GLsizeiptr indices_size) {
	ib_bind(ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
	ib_unbind(ib);
}

void gl_ib_init_dynamic(GLuint ib, GLsizeiptr indices_size) {
	ib_bind(ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, NULL, GL_DYNAMIC_DRAW);
	ib_unbind(ib);
}

void gl_ib_set_data(GLuint ib, const void* indices, GLsizeiptr indices_size) {
	ib_bind(ib);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices_size, indices);
}

void gl_ib_delete(GLuint ib) {
	glDeleteBuffers(1, &ib);
}
#endif