#include "gl_error.h"

#include "utils/str.h"

GLError *gl_error_create(GLError *error)
{
#ifdef _DEBUG
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0) {
		log_error("debugging is disabled");
		return error;
	}
#endif

	error->max_len = 0;
	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &error->max_len);
	error->max_count = 0;
	glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES, &error->max_count);

	error->sources	  = m_malloc(error->max_count * sizeof(GLenum));
	error->types	  = m_malloc(error->max_count * sizeof(GLenum));
	error->ids	  = m_malloc(error->max_count * sizeof(GLuint));
	error->severities = m_malloc(error->max_count * sizeof(GLenum));
	error->lengths	  = m_malloc(error->max_count * sizeof(GLsizei));

	str_create(&error->msgs, error->max_count * error->max_len);
	str_create(&error->text, 2048);
	str_create(&error->info, 2048);

	return error;
}

void gl_error_begin(GLError *error)
{
}

static const char *get_severity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW: return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "";
}

static const char *get_type(GLenum type)
{
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

static char *get_info(GLError *error, GLuint num_msgs)
{
	str_clear(&error->info);
	str_off_reset(&error->msgs);
	for (GLuint i = 0; i < num_msgs; i++) {
		printf("Id: 0x%X\n", error->ids[i]);
		str_add_cstrf(&error->info, "%s %s %u: ", get_severity(error->severities[i]), get_type(error->types[i]), error->ids[i]);
		str_add_strn(&error->info, &error->msgs, error->lengths[i]);
		str_add_nl(&error->info);
		str_off_add(&error->msgs, error->lengths[i]);
	}
	str_off_reset(&error->msgs);
	return error->info.data;
}

bool gl_error_failed(GLError *error, const char *msg, const char *fn, const char *func, int line)
{
	GLuint num_msgs =
		glGetDebugMessageLog(error->max_count, error->msgs.count, error->sources, error->types, error->ids, error->severities, error->lengths, error->msgs.data);
	if (num_msgs > 0) {
		str_clear(&error->text);

		str_add_cstrf(&error->text,
			      "%s\n"
			      "INFO:\n%s\n",
			      fn, get_info(error, num_msgs));

		log_error("%s: %s", msg, error->text.data);
		return 0;
	}
	return 1;
}

bool gl_error_assert(GLError *error, const char *fn, const char *func, int line)
{
	GLuint num_msgs =
		glGetDebugMessageLog(error->max_count, error->msgs.count, error->sources, error->types, error->ids, error->severities, error->lengths, error->msgs.data);
	if (num_msgs > 0) {
		str_clear(&error->text);

		str_add_cstrf(&error->text,
			      "%s\n"
			      "INFO:\n%s\n",
			      fn, get_info(error, num_msgs));

		log_error(error->text.data);
		return 0;
	}
	return 1;
}

void gl_error_delete(GLError *error)
{
	m_free(error->sources, error->max_count * sizeof(GLenum));
	m_free(error->types, error->max_count * sizeof(GLenum));
	m_free(error->ids, error->max_count * sizeof(GLuint));
	m_free(error->severities, error->max_count * sizeof(GLenum));
	m_free(error->lengths, error->max_count * sizeof(GLsizei));
	str_delete(&error->msgs);
	str_delete(&error->text);
	str_delete(&error->info);
}
