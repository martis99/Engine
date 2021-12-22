#pragma once
#include "eng_types.h"

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

typedef struct AContext AContext;
typedef struct AUniformBuffer AUniformBuffer;
typedef struct ATexture ATexture;
typedef struct ARenderer ARenderer;
typedef struct AShader AShader;
typedef struct ARenderer ARenderer;
typedef struct AMesh AMesh;
typedef struct AFramebuffer AFramebuffer;

typedef struct ABuffer {
	uint* offsets;
	uint* sizes;
	uint size;
	uint count;
	void* data;
} ABuffer;

typedef struct AValue {
	AType type;
	char name[20];
} AValue;

typedef enum ABufferType {
	A_BFR_UNKNOWN,
	A_BFR_VERTEX,
	A_BFR_INSTANCE,
	A_BFR_INDEX,
	A_BFR_GLOBAL,
	A_BFR_VS,
	A_BFR_PS,
	A_BFR_PS_OUT
} ABufferType;

typedef struct ABufferDesc {
	ABufferType type;
	uint slot;
	AValue* values;
	uint values_size;
	uint max_count;
	char name[20];
} ABufferDesc;

typedef struct AShaderDesc {
	ABufferDesc* buffers;
	uint buffers_size;
	uint textures_count;
	AType texture_type;
} AShaderDesc;

typedef struct ABufferData {
	void* data;
	uint size;
} ABufferData;

typedef struct AMeshData {
	ABufferData vertices;
	ABufferData instances;
	ABufferData indices;
} AMeshData;

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

uint abufferdesc_size(ABufferDesc* desc);
uint abufferdesc_count(ABufferDesc* desc, bool mat_support);
void abufferdesc_copy(ABufferDesc* src, ABufferDesc* dst);
void abufferdesc_delete(ABufferDesc* desc);

void ashaderdesc_copy(AShaderDesc* src, AShaderDesc* dst);
void ashaderdesc_delete(AShaderDesc* desc);
ABufferDesc* ashaderdesc_get_bufferdesc(AShaderDesc desc, ABufferType type);