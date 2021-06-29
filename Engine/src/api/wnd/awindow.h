#pragma once

typedef struct AWindowCallbacks {
	void(*key_pressed)(byte);
	void(*key_released)(byte);
} AWindowCallbacks;

typedef struct AWindow AWindow;

AWindow* awindow_create(AWindowCallbacks* callbacks, int width, int height);
void awindow_delete(AWindow* window);

void awindow_set_title(AWindow* window, const char* title);
int awindow_poll_events(AWindow* window);
void awindow_close(AWindow* window);