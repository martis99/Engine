#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_enums.h"

GLenum gl_aprimitive(APrimitive primitive) {
	switch (primitive) {
	case A_POINTS: return GL_POINTS;
	case A_LINES: return GL_LINES;
	case A_TRIANGLES: return GL_TRIANGLES;
	case A_QUADS: return GL_QUADS;
	}
	return 0;
}

GLenum gl_ashadertype(AShaderType type) {
	switch (type) {
	case A_FRAGMENT: return GL_FRAGMENT_SHADER;
	case A_VERTEX: return GL_VERTEX_SHADER;
	case A_GEOMETRY: return GL_GEOMETRY_SHADER;
	case A_COMPUTE: return GL_COMPUTE_SHADER;
	}
	return 0;
}

GLint gl_awrap(AWrap wrap) {
	switch (wrap) {
	case A_REPEAT: return GL_REPEAT;
	case A_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
	}
	return 0;
}

GLint gl_afilter(AFilter filter) {
	switch (filter) {
	case A_NEAREST: return GL_NEAREST;
	case A_LINEAR: return GL_LINEAR;
	}
	return 0;
}

GLenum gl_afactor(AFactor factor) {
	switch (factor) {
	case A_ZERO: return GL_ZERO;
	case A_ONE: return GL_ONE;
	case A_SRC_COLOR: return GL_SRC_COLOR;
	case A_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
	case A_SRC_ALPHA: return GL_SRC_ALPHA;
	case A_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	case A_DST_ALPHA: return  GL_DST_ALPHA;
	case A_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
	case A_DST_COLOR: return GL_DST_COLOR;
	case A_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
	}
	return 0;
}
#endif