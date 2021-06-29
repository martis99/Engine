#pragma once

typedef struct AWindow AWindow;

AWindow* awindow_create(int width, int height);
void awindow_delete(AWindow* window);

void awindow_set_title(AWindow* window, const char* title);
int awindow_poll_events(AWindow* window);