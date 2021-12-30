#pragma once
#include "dx11.h"
#include "dx11/dx11_atypes.h"

DX11Error* dx11_error_create(DX11Error* error, AErrorCallbacks* callbacks);
void dx11_error_begin(DX11Error* error);
bool dx11_error_failed(DX11Error* error, const char* msg, HRESULT hr, const char* fn, const char* file, int line);
bool dx11_error_assert(DX11Error* error, HRESULT hr, const char* fn, const char* file, int line);
void dx11_error_delete(DX11Error* error);

#define DX11_FAILED(error, msg, hr) dx11_error_begin(error), dx11_error_failed(error, msg, hr, #hr, __FILE__, __LINE__) == 0
#define DX11_ASSERT(error, hr) dx11_error_begin(error), dx11_error_assert(error, hr, #hr, __FILE__, __LINE__) == 0