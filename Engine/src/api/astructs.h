#pragma once
#include "aenums.h"

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