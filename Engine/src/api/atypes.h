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

typedef enum AType {
	VEC1U,
	VEC2U,
	VEC3U,
	VEC4U,
	VEC1I,
	VEC2I,
	VEC3I,
	VEC4I,
	VEC1F,
	VEC2F,
	VEC3F,
	VEC4F,
	MAT4F
} AType;

typedef struct AWindow AWindow;
typedef struct ACursor ACursor;
typedef struct AContext AContext;

typedef struct AWindowCallbacks {
	void(*key_pressed)(byte);
	void(*key_released)(byte);
	void(*mouse_pressed)(byte);
	void(*mouse_released)(byte);
	void(*mouse_moved)(float, float);
	void(*mouse_moved_delta)(float, float);
	void(*mouse_wheel)(float);
} AWindowCallbacks;

typedef struct AUniformBuffer AUniformBuffer;
typedef struct ATexture ATexture;
typedef struct ARenderer ARenderer;
typedef struct AShader AShader;
typedef struct ARenderer ARenderer;
typedef struct AMesh AMesh;
typedef struct AFramebuffer AFramebuffer;
typedef struct ABuffer ABuffer;
typedef struct AMaterial AMaterial;

typedef struct AValue {
	const char* name;
	AType type;
} AValue;

typedef struct ABufferDesc {
	bool enabled;
	AValue* layout;
	uint layout_size;
	const void* data;
	uint data_size;
} ABufferDesc;

typedef struct AMeshDesc {
	ABufferDesc vertices;
	ABufferDesc instances;
	ABufferDesc indices;
} AMeshDesc;

int atype_components(AType type, bool mat_support);
uint atype_component_size(AType type);
uint atype_size(AType type);
uint atype_count(AType type, bool mat_support);

uint abufferdesc_size(ABufferDesc desc);
uint abufferdesc_count(ABufferDesc desc, bool mat_support);
void abufferdesc_copy(ABufferDesc* dst, ABufferDesc* src);
void abufferdesc_delete(ABufferDesc* desc);

void ameshdesc_copy(AMeshDesc* dst, AMeshDesc* src);
void ameshdesc_delete(AMeshDesc* desc);