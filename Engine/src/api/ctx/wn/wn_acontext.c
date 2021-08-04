#include "pch.h"
#ifdef E_WINDOWS
#ifdef GAPI_NONE
#include "api/ctx/acontext.h"
#include "api/wnd/awindow.h"
#include "api/gfx/none/n_atypes.h"

#include <Windows.h>

AContext* acontext_create(AWindow* window) {
	AContext* context = m_malloc(sizeof(AContext));
	context->window = awindow_get_window(window);
	context->device = GetDC(context->window);
	return context;
}

void acontext_delete(AContext* context) {
	ReleaseDC(context->window, context->device);
	m_free(context, sizeof(AContext));
}

void acontext_swap_buffers(AContext* context) {

}
#endif
#endif