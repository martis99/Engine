#pragma once
#include "eng_common_types.h"
#include "log.h"
#include "mem.h"

typedef struct AWindow AWindow;
typedef struct ACursor ACursor;

typedef struct AWindowCallbacks {
	void (*key_pressed)(void *priv, byte key);
	void (*key_released)(void *priv, byte key);
	void (*mouse_pressed)(void *priv, byte button);
	void (*mouse_released)(void *priv, byte button);
	void (*mouse_moved)(void *priv, float x, float y);
	void (*mouse_moved_delta)(void *priv, float dx, float dy);
	void (*mouse_wheel)(void *priv, float delta);
	void *priv;
} AWindowCallbacks;
