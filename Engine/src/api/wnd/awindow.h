#pragma once

typedef struct AWindow AWindow;
typedef struct ACursor ACursor;

typedef struct AWindowCallbacks {
	void(*key_pressed)(byte);
	void(*key_released)(byte);
	void(*mouse_pressed)(byte);
	void(*mouse_released)(byte);
	void(*mouse_moved)(float, float);
	void(*mouse_moved_delta)(float, float);
	void(*mouse_wheel)(float);
} AWindowCallbacks;

AWindow* awindow_create(AWindowCallbacks* callbacks, ACursor* cursor, int width, int height);
void awindow_delete(AWindow* window);

void* awindow_get_window(AWindow* window);

void awindow_set_title(AWindow* window, const char* title);
int awindow_poll_events(AWindow* window);
void awindow_close(AWindow* window);