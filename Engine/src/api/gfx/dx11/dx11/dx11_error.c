#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_error.h"
#include "dxerr/dxerr.h"

#include "app.h"
#include "window/window.h"
#include "utils/str.h"

#include <dxgidebug.h>

static HMODULE s_library;
static IDXGIInfoQueue* s_info_queue;
static UINT64 s_begin;

static WStr s_caption;
static WStr s_text;
static WStr s_desc;
static Str s_info;

void dx11_error_create() {
	s_library = LoadLibraryA("dxgidebug.dll");
	if (s_library == NULL) {
		show_error("Failed to load library", "Error");
		return;
	}

	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	DXGIGetDebugInterface debug_interface = (DXGIGetDebugInterface)GetProcAddress(s_library, "DXGIGetDebugInterface");
	if (debug_interface == NULL) {
		show_error("Failed to get process address", "Error");
		return;
	}

	HRESULT hr = debug_interface(&IID_IDXGIInfoQueue, &s_info_queue);

	wstr_create(&s_caption, 128);
	wstr_create(&s_text, 2048);
	wstr_create(&s_desc, 2048);
	str_create(&s_info, 2048);
}

void dx11_error_begin() {
	s_begin = s_info_queue->lpVtbl->GetNumStoredMessages(s_info_queue, DXGI_DEBUG_ALL);
}

static const char* get_severity(DXGI_INFO_QUEUE_MESSAGE_SEVERITY severity) {
	switch (severity) {
	case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION: return "CORRUPTION";
	case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR: return "ERROR";
	case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING: return "WARNING";
	case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO: return "INFO";
	case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE: return "MESSAGE";
	}
	return "";
}

static const char* get_category(DXGI_INFO_QUEUE_MESSAGE_CATEGORY category) {
	switch (category) {
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN: return "UNKNOWN";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS: return "MISCELLANEOUS";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION: return "INITIALIZATION";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP: return "CLEANUP";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION: return "COMPILATION";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION: return "STATE CREATION";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING: return "STATE SETTING";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING: return "STATE GETTING";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION: return "RESOURCE MANIPULATION";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION: return "EXECUTION";
	case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER: return "SHADER";
	}
	return "";
}

static char* get_info() {
	str_zero(&s_info);

	UINT64 end = s_info_queue->lpVtbl->GetNumStoredMessages(s_info_queue, DXGI_DEBUG_ALL);
	for (UINT64 i = s_begin; i < end; i++) {
		SIZE_T messageLength;
		HRESULT hr = s_info_queue->lpVtbl->GetMessageW(s_info_queue, DXGI_DEBUG_ALL, i, NULL, &messageLength);
		if (FAILED(hr)) {
			show_error("Failed to get message length", "Error");
			return NULL;
		}

		DXGI_INFO_QUEUE_MESSAGE* msg = m_malloc(messageLength);
		hr = s_info_queue->lpVtbl->GetMessageW(s_info_queue, DXGI_DEBUG_ALL, i, msg, &messageLength);
		if (FAILED(hr)) {
			show_error("Failed to get message", "Error");
			return NULL;
		}

		str_catf(&s_info, "%s %s #%i: ", get_category(msg->Category), get_severity(msg->Severity), msg->ID);
		str_catc(&s_info, msg->pDescription, (uint)msg->DescriptionByteLength);
		str_nl(&s_info);
		m_free(msg, messageLength);
	}
	return s_info.data;
}

bool dx11_error_failed(const char* msg, HRESULT hr, const char* fn, const char* file, int line) {
	if (FAILED(hr)) {
		wstr_zero(&s_text);
		DXGetErrorDescriptionW(hr, s_desc.data, s_desc.count);

#ifdef _DEBUG
		wstr_catf(&s_text,
			L"%ls (0x%X (%lu))\n"
			L"%hs\n\n"
			L"DESC:\n%ls\n"
			L"INFO:\n%hs\n"
			L"%hs: %i\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, fn, s_desc.data, get_info(), file, line);
#elif NDEBUG
		wstr_catf(&s_text,
			L"%ls (0x%X (%lu))\n\n"
			L"DESC:\n%ls\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, s_desc.data);
#endif

		wstr_zero(&s_caption);
		wstr_catf(&s_caption, L"%hs", msg);
		show_errorw(s_text.data, s_caption.data);
		return 0;
	}

	return 1;
}

bool dx11_error_assert(HRESULT hr, const char* fn, const char* file, int line) {
	if (FAILED(hr)) {
		wstr_zero(&s_text);
		DXGetErrorDescriptionW(hr, s_desc.data, s_desc.count);

#ifdef _DEBUG
		wstr_catf(&s_text,
			L"%ls (0x%X (%lu))\n"
			L"%hs\n\n"
			L"DESC:\n%ls\n"
			L"INFO:\n%hs\n"
			L"%hs: %i\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, fn, s_desc.data, get_info(), file, line);
#elif NDEBUG
		wstr_catf(&s_text,
			L"%ls (0x%X (%lu))\n\n"
			L"DESC:\n%ls\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, s_desc.data);
#endif

		wstr_zero(&s_caption);
		show_errorw(s_text.data, L"Error");
		return 0;
	}

	return 1;
}

void dx11_error_delete() {
	wstr_delete(&s_caption);
	wstr_delete(&s_text);
	wstr_delete(&s_desc);
	str_delete(&s_info);

	if (s_info_queue != NULL) {
		s_info_queue->lpVtbl->Release(s_info_queue);
	}
	FreeLibrary(s_library);
}

#endif