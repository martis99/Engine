#include "pch.h"
#include "api/ctx/acontext.h"
#include "none/gn_atypes.h"

#include <Windows.h>

typedef struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
} AWindow;

AContext* acontext_create(void* window) {
	AWindow* awindow = window;
	AContext* context = m_malloc(sizeof(AContext));
	context->window = awindow->window;
	context->device = GetDC(context->window);
	return context;
}

void acontext_delete(AContext* context) {
	ReleaseDC(context->window, context->device);
	m_free(context, sizeof(AContext));
}

void acontext_swap_buffers(AContext* context) {

}