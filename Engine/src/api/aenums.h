#pragma once

typedef enum APrimitive {
	A_POINTS,
	A_LINES,
	A_TRIANGLES,
	A_QUADS
} APrimitive;

typedef enum AShaderType {
	A_FRAGMENT,
	A_VERTEX,
	A_GEOMETRY,
	A_COMPUTE
} AShaderType;

typedef enum AWrap {
	A_REPEAT,
	A_CLAMP_TO_EDGE
} AWrap;

typedef enum AFilter {
	A_NEAREST,
	A_LINEAR
} AFilter;

typedef enum AFactor {
	A_ZERO,
	A_ONE,
	A_SRC_COLOR,
	A_ONE_MINUS_SRC_COLOR,
	A_DST_COLOR,
	A_ONE_MINUS_DST_COLOR,
	A_SRC_ALPHA,
	A_ONE_MINUS_SRC_ALPHA,
	A_DST_ALPHA,
	A_ONE_MINUS_DST_ALPHA
} AFactor;

typedef enum AAttachmentFormat {
	A_RGBA8,
	A_RED_INTEGER,
	A_DEPTH24STENCIL8
} AAttachmentFormat;

typedef enum {
	EMPTY,
	VEC1I,
	VEC2I,
	VEC3I,
	VEC4I,
	VEC1F,
	VEC2F,
	VEC3F,
	VEC4F,
	MAT4F
} ADataType;