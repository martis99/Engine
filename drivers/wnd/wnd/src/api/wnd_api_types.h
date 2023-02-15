#pragma once
#include "eng_common_types.h"
#include "log.h"
#include "mem.h"

typedef struct AWindow AWindow;
typedef struct ACursor ACursor;

typedef struct AWindowCallbacks {
	void (*key_pressed)(void *arg, byte key);
	void (*key_released)(void *arg, byte key);
	void (*mouse_pressed)(void *arg, byte button);
	void (*mouse_released)(void *arg, byte button);
	void (*mouse_moved)(void *arg, float x, float y);
	void (*mouse_moved_delta)(void *arg, float dx, float dy);
	void (*mouse_wheel)(void *arg, float delta);
	void *arg;
} AWindowCallbacks;
