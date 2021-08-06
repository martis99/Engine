#pragma once

typedef enum APrimitive {
	A_PRIMITIVE_UNKNOWN,
	A_POINTS,
	A_LINES,
	A_TRIANGLES,
	A_QUADS
} APrimitive;

typedef enum AShaderType {
	A_SHADER_TYOE_UNKNOWN,
	A_FRAGMENT,
	A_VERTEX,
	A_GEOMETRY,
	A_COMPUTE
} AShaderType;

typedef enum AWrap {
	A_WRAP_UNKNOWN,
	A_REPEAT,
	A_CLAMP_TO_EDGE
} AWrap;

typedef enum AFilter {
	A_FILTER_UNKNOWN,
	A_NEAREST,
	A_LINEAR
} AFilter;

typedef enum AFactor {
	A_FACTOR_UNKNOWN,
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

typedef enum AType {
	A_TYPE_UNKOWN,
	VEC1B,
	VEC2B,
	VEC3B,
	VEC4B,
	VEC1UB,
	VEC2UB,
	VEC3UB,
	VEC4UB,
	VEC1S,
	VEC2S,
	VEC3S,
	VEC4S,
	VEC1US,
	VEC2US,
	VEC3US,
	VEC4US,
	VEC1I,
	VEC2I,
	VEC3I,
	VEC4I,
	VEC1UI,
	VEC2UI,
	VEC3UI,
	VEC4UI,
	VEC1F,
	VEC2F,
	VEC3F,
	VEC4F,
	VEC1D,
	VEC2D,
	VEC3D,
	VEC4D,
	MAT4F
} AType;

typedef enum ATypeType {
	A_TYPE_TYPE_UNKNOWN,
	A_B,
	A_UB,
	A_S,
	A_US,
	A_I,
	A_UI,
	A_F,
	A_D
} ATypeType;

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

typedef struct AAttachmentDesc {
	AType type;
	bool readable;
	AFilter filter;
	AWrap wrap;
} AAttachmentDesc;

ATypeType atype_type(AType type);
uint atype_components(AType type, bool mat_support);
uint atype_component_size(AType type);
uint atype_size(AType type);
uint atype_count(AType type, bool mat_support);
void atype_convert(float* dst, const void* src, AType type);

uint abufferdesc_size(ABufferDesc desc);
uint abufferdesc_count(ABufferDesc desc, bool mat_support);
void abufferdesc_copy(ABufferDesc* dst, ABufferDesc* src);
void abufferdesc_delete(ABufferDesc* desc);

void ameshdesc_copy(AMeshDesc* dst, AMeshDesc* src);
void ameshdesc_delete(AMeshDesc* desc);