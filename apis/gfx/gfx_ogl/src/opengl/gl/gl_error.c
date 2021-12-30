#include "gl_error.h"

#include "app.h"
#include "utils/str.h"

GLError* gl_error_create(GLError* error, AErrorCallbacks* callbacks) {
	error->callbacks = *callbacks;
#ifdef _DEBUG
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0) {
		callbacks->on_error("Debugging is disabled", "Error");
		return NULL;
	}
#endif

	error->max_len = 0;
	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &error->max_len);
	error->max_count = 0;
	glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES, &error->max_count);

	error->sources = m_malloc(error->max_count * sizeof(GLenum));
	error->types = m_malloc(error->max_count * sizeof(GLenum));
	error->ids = m_malloc(error->max_count * sizeof(GLuint));
	error->severities = m_malloc(error->max_count * sizeof(GLenum));
	error->lengths = m_malloc(error->max_count * sizeof(GLsizei));

	str_create(&error->msgs, error->max_count * error->max_len);
	str_create(&error->text, 2048);
	str_create(&error->info, 2048);

	return error;
}

void gl_error_begin(GLError* error) {

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

static char* get_info(GLError* error, GLuint num_msgs) {
	str_zero(&error->info);

	GLuint curpos = 0;
	for (GLuint i = 0; i < num_msgs; i++) {
		printf("Id: 0x%X\n", error->ids[i]);
		str_catf(&error->info, "%s %s %u: ", get_severity(error->severities[i]), get_type(error->types[i]), error->ids[i]);
		str_cats(&error->info, str_view(&error->msgs, curpos, error->lengths[i]));
		str_nl(&error->info);
		curpos += error->lengths[i];
	}
	return error->info.data;
}

bool gl_error_failed(GLError* error, const char* msg, const char* fn, const char* file, int line) {
	GLuint num_msgs = glGetDebugMessageLog(error->max_count, error->msgs.count, error->sources, error->types, error->ids, error->severities, error->lengths, error->msgs.data);
	if (num_msgs > 0) {
		str_zero(&error->text);

		str_catf(&error->text,
			"%s\n"
			"INFO:\n%s\n"
			"%s: %i\n",
			fn, get_info(error, num_msgs), file, line);

		error->callbacks.on_error(error->text.data, msg);
		return 0;
	}
	return 1;
}

bool gl_error_assert(GLError* error, const char* fn, const char* file, int line) {
	GLuint num_msgs = glGetDebugMessageLog(error->max_count, error->msgs.count, error->sources, error->types, error->ids, error->severities, error->lengths, error->msgs.data);
	if (num_msgs > 0) {
		str_zero(&error->text);

		str_catf(&error->text,
			"%s\n"
			"INFO:\n%s\n"
			"%s: %i\n",
			fn, get_info(error, num_msgs), file, line);

		error->callbacks.on_error(error->text.data, "Error");
		return 0;
	}
	return 1;
}

void gl_error_delete(GLError* error) {
	m_free(error->sources, error->max_count * sizeof(GLenum));
	m_free(error->types, error->max_count * sizeof(GLenum));
	m_free(error->ids, error->max_count * sizeof(GLuint));
	m_free(error->severities, error->max_count * sizeof(GLenum));
	m_free(error->lengths, error->max_count * sizeof(GLsizei));
	str_delete(&error->msgs);
	str_delete(&error->text);
	str_delete(&error->info);
}