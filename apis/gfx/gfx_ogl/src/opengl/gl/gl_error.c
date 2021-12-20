#include "pch.h"
#include "gl_error.h"

#include "app.h"
#include "window/window.h"
#include "utils/str.h"

static GLint s_max_len;
static GLint s_max_count;
static GLenum* s_sources;
static GLenum* s_types;
static GLuint* s_ids;
static GLenum* s_severities;
static GLsizei* s_lengths;

static Str s_msgs;
static Str s_text;
static Str s_info;

void gl_error_create() {
#ifdef _DEBUG
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0) {
		show_error("Debugging is disabled", "Error");
		return;
	}
#endif

	s_max_len = 0;
	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &s_max_len);
	s_max_count = 0;
	glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES, &s_max_count);

	s_sources = m_malloc(s_max_count * sizeof(GLenum));
	s_types = m_malloc(s_max_count * sizeof(GLenum));
	s_ids = m_malloc(s_max_count * sizeof(GLuint));
	s_severities = m_malloc(s_max_count * sizeof(GLenum));
	s_lengths = m_malloc(s_max_count * sizeof(GLsizei));

	str_create(&s_msgs, s_max_count * s_max_len);
	str_create(&s_text, 2048);
	str_create(&s_info, 2048);
}

void gl_error_begin() {

}

static const char* get_severity(GLenum severity) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW: return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "";
}

static const char* get_type(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
	case GL_DEBUG_TYPE_MARKER: return "MARKER";
	case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH GROUP";
	case GL_DEBUG_TYPE_POP_GROUP: return "POP GROUP";
	case GL_DEBUG_TYPE_OTHER: return "OTHER";
	}
	return "";
}

static char* get_info(GLuint num_msgs) {
	str_zero(&s_info);

	GLuint curpos = 0;
	for (GLuint i = 0; i < num_msgs; i++) {
		printf("Id: 0x%X\n", s_ids[i]);
		str_catf(&s_info, "%s %s %u: ", get_severity(s_severities[i]), get_type(s_types[i]), s_ids[i]);
		str_cats(&s_info, str_view(&s_msgs, curpos, s_lengths[i]));
		str_nl(&s_info);
		curpos += s_lengths[i];
	}
	return s_info.data;
}

bool gl_error_failed(const char* msg, const char* fn, const char* file, int line) {
	GLuint num_msgs = glGetDebugMessageLog(s_max_count, s_msgs.count, s_sources, s_types, s_ids, s_severities, s_lengths, s_msgs.data);
	if (num_msgs > 0) {
		str_zero(&s_text);

		str_catf(&s_text,
			"%s\n"
			"INFO:\n%s\n"
			"%s: %i\n",
			fn, get_info(num_msgs), file, line);

		show_error(s_text.data, msg);
		return 0;
	}
	return 1;
}

bool gl_error_assert(const char* fn, const char* file, int line) {
	GLuint num_msgs = glGetDebugMessageLog(s_max_count, s_msgs.count, s_sources, s_types, s_ids, s_severities, s_lengths, s_msgs.data);
	if (num_msgs > 0) {
		str_zero(&s_text);

		str_catf(&s_text,
			"%s\n"
			"INFO:\n%s\n"
			"%s: %i\n",
			fn, get_info(num_msgs), file, line);

		show_error(s_text.data, "Error");
		return 0;
	}
	return 1;
}

void gl_error_delete() {
	m_free(s_sources, s_max_count * sizeof(GLenum));
	m_free(s_types, s_max_count * sizeof(GLenum));
	m_free(s_ids, s_max_count * sizeof(GLuint));
	m_free(s_severities, s_max_count * sizeof(GLenum));
	m_free(s_lengths, s_max_count * sizeof(GLsizei));
	str_delete(&s_msgs);
	str_delete(&s_text);
	str_delete(&s_info);
}