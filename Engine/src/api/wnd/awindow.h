#pragma once

typedef struct AWindow AWindow;

AWindow* awindow_create(int width, int height);
void awindow_delete(AWindow* window);