#pragma once
#include <Windows.h>
#include <sal.h>

const WCHAR *WINAPI DXGetErrorStringW(_In_ HRESULT hr);
void WINAPI DXGetErrorDescriptionW(_In_ HRESULT hr, _Out_cap_(count) WCHAR *desc, _In_ size_t count);
