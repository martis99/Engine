#include "dx11_error.h"
#include "dxerr/dxerr.h"

#include "app.h"
#include "utils/str.h"

DX11Error* dx11_error_create(DX11Error* error, AErrorCallbacks* callbacks) {
	error->library = LoadLibraryA("dxgidebug.dll");
	error->callbacks = *callbacks;

	if (error->library == NULL) {
		error->callbacks.on_error("Failed to load library", "Error");
		return NULL;
	}

	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	DXGIGetDebugInterface debug_interface = (DXGIGetDebugInterface)GetProcAddress(error->library, "DXGIGetDebugInterface");
	if (debug_interface == NULL) {
		error->callbacks.on_error("Failed to get process address", "Error");
		return NULL;
	}

	HRESULT hr = debug_interface(&IID_IDXGIInfoQueue, &error->info_queue);

	wstr_create(&error->caption, 128);
	wstr_create(&error->text, 2048);
	wstr_create(&error->desc, 2048);
	str_create(&error->info, 2048);

	return error;
}

void dx11_error_begin(DX11Error* error) {
	error->begin = error->info_queue->lpVtbl->GetNumStoredMessages(error->info_queue, DXGI_DEBUG_ALL);
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

static char* get_info(DX11Error* error) {
	str_zero(&error->info);

	UINT64 end = error->info_queue->lpVtbl->GetNumStoredMessages(error->info_queue, DXGI_DEBUG_ALL);
	for (UINT64 i = error->begin; i < end; i++) {
		SIZE_T messageLength;
		HRESULT hr = error->info_queue->lpVtbl->GetMessageW(error->info_queue, DXGI_DEBUG_ALL, i, NULL, &messageLength);
		if (FAILED(hr)) {
			error->callbacks.on_error("Failed to get message length", "Error");
			return NULL;
		}

		DXGI_INFO_QUEUE_MESSAGE* msg = m_malloc(messageLength);
		hr = error->info_queue->lpVtbl->GetMessageW(error->info_queue, DXGI_DEBUG_ALL, i, msg, &messageLength);
		if (FAILED(hr)) {
			error->callbacks.on_error("Failed to get message", "Error");
			return NULL;
		}

		str_catf(&error->info, "%s %s #%i: ", get_category(msg->Category), get_severity(msg->Severity), msg->ID);
		str_catc(&error->info, msg->pDescription, (uint)msg->DescriptionByteLength);
		str_nl(&error->info);
		m_free(msg, messageLength);
	}
	return error->info.data;
}

bool dx11_error_failed(DX11Error* error, const char* msg, HRESULT hr, const char* fn, const char* file, int line) {
	if (FAILED(hr)) {
		wstr_zero(&error->text);
		DXGetErrorDescriptionW(hr, error->desc.data, error->desc.count);

#ifdef _DEBUG
		wstr_catf(&error->text,
			L"%ls (0x%X (%lu))\n"
			L"%hs\n\n"
			L"DESC:\n%ls\n"
			L"INFO:\n%hs\n"
			L"%hs: %i\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, fn, error->desc.data, get_info(error), file, line);
#elif NDEBUG
		wstr_catf(&s_text,
			L"%ls (0x%X (%lu))\n\n"
			L"DESC:\n%ls\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, s_desc.data);
#endif

		wstr_zero(&error->caption);
		wstr_catf(&error->caption, L"%hs", msg);
		error->callbacks.on_errorw(error->text.data, error->caption.data);
		return 0;
	}

	return 1;
}

bool dx11_error_assert(DX11Error* error, HRESULT hr, const char* fn, const char* file, int line) {
	if (FAILED(hr)) {
		wstr_zero(&error->text);
		DXGetErrorDescriptionW(hr, error->desc.data, error->desc.count);

#ifdef _DEBUG
		wstr_catf(&error->text,
			L"%ls (0x%X (%lu))\n"
			L"%hs\n\n"
			L"DESC:\n%ls\n"
			L"INFO:\n%hs\n"
			L"%hs: %i\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, fn, error->desc.data, get_info(error), file, line);
#elif NDEBUG
		wstr_catf(&s_text,
			L"%ls (0x%X (%lu))\n\n"
			L"DESC:\n%ls\n",
			DXGetErrorStringW(hr), (unsigned long)hr, (unsigned long)hr, s_desc.data);
#endif

		wstr_zero(&error->caption);
		error->callbacks.on_errorw(error->text.data, L"Error");
		return 0;
	}

	return 1;
}

void dx11_error_delete(DX11Error* error) {
	wstr_delete(&error->caption);
	wstr_delete(&error->text);
	wstr_delete(&error->desc);
	str_delete(&error->info);

	if (error->info_queue != NULL) {
		error->info_queue->lpVtbl->Release(error->info_queue);
	}
	FreeLibrary(error->library);
}