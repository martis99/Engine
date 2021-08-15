#pragma once
#ifdef GAPI_DX11
#include "dx11.h"

void dx11_error_create();
void dx11_error_begin();
bool dx11_error_failed(const char* msg, HRESULT hr, const char* fn, const char* file, int line);
bool dx11_error_assert(HRESULT hr, const char* fn, const char* file, int line);
void dx11_error_delete();

#define DX11_FAILED(msg, hr) dx11_error_begin(), dx11_error_failed(msg, hr, #hr, __FILE__, __LINE__) == 0
#define DX11_ASSERT(hr) dx11_error_begin(), dx11_error_assert(hr, #hr, __FILE__, __LINE__) == 0

#endif