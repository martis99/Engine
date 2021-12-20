#include "pch.h"
#include "str.h"

void str_create(Str* str, uint count) {
	str->data = malloc(count * sizeof(char));
	str->count = count;
}

void wstr_create(WStr* str, uint count) {
	str->data = m_malloc(count * sizeof(wchar));
	str->count = count;
}

void str_delete(Str* str) {
	free(str->data);
}

void wstr_delete(WStr* str) {
	m_free(str->data, str->count * sizeof(wchar));
}

void str_resize(Str* str, uint new_count) {
	if (new_count > str->count) {
		str->data = m_realloc(str->data, new_count * sizeof(char), str->count * sizeof(char));
		str->count = new_count;
	}
}

void wstr_resize(WStr* str, uint new_count) {
	if (new_count > str->count) {
		str->data = m_realloc(str->data, new_count * sizeof(wchar), str->count * sizeof(wchar));
		str->count = new_count;
	}
}

void str_increase(Str* str, uint count) {
	str_resize(str, ((uint)strlen(str->data) + count) * sizeof(char));
}

void wstr_increase(WStr* str, uint count) {
	wstr_resize(str, ((uint)wcslen(str->data) + count) * sizeof(wchar));
}

void str_zero(Str* str) {
	memset(str->data, 0, str->count * sizeof(char));
}

void wstr_zero(WStr* str) {
	memset(str->data, 0, str->count * sizeof(wchar));
}

void str_nl(Str* str) {
	str_cat(str, "\n");
}

void wstr_nl(WStr* str) {
	wstr_cat(str, L"\n");
}

void str_catf(Str* str, const char* format, ...) {
	va_list args;
	va_start(args, format);
	vsprintf_s(str->data + strlen(str->data), str->count - strlen(str->data), format, args);
	va_end(args);
}

void wstr_catf(WStr* str, const wchar* format, ...) {
	va_list args;
	va_start(args, format);
	vswprintf_s(str->data + wcslen(str->data), str->count - wcslen(str->data), format, args);
	va_end(args);
}

void str_cat(Str* str, const char* data) {
	str_catc(str, data, (uint)strlen(data));
}

void wstr_cat(WStr* str, const wchar* data) {
	wstr_catc(str, data, (uint)wcslen(data));
}

void str_catch(Str* str, char data) {
	str_increase(str, 1);
	str->data[strlen(str->data)] = data;
}

void wstr_catch(WStr* str, wchar data) {
	wstr_increase(str, 1);
	str->data[wcslen(str->data)] = data;
}

void str_catc(Str* str, const char* data, uint count) {
	str_increase(str, count);
	strncat_s(str->data, str->count, data, count);
}

void wstr_catc(WStr* str, const wchar* data, uint count) {
	wstr_increase(str, count);
	wcsncat_s(str->data, str->count, data, count);
}

void str_cats(Str* str, Str src) {
	str_catc(str, src.data, src.count);
}

void wstr_catws(WStr* str, WStr src) {
	wstr_catc(str, src.data, src.count);
}

void str_cati(Str* str, int value) {
	char str_val[10] = { 0 };
	_itoa_s(value, str_val, 10, 10);
	str_cat(str, str_val);
}

void wstr_cati(WStr* str, int value) {
	wchar str_val[10] = { 0 };
	_itow_s(value, str_val, 10, 10);
	wstr_cat(str, str_val);
}

void str_cpy(Str* str, char* dst) {
	memcpy(dst, str->data, str->count * sizeof(char));
}

void wstr_cpy(WStr* str, wchar* dst) {
	memcpy(dst, str->data, str->count * sizeof(wchar));
}

int str_cmp(Str* str, const char* data) {
	return strcmp(str->data, data);
}

int wstr_cmp(WStr* str, const wchar* data) {
	return wcscmp(str->data, data);
}

Str str_view(Str* str, uint start, uint length) {
	return (Str) { str->data + start, length };
}

WStr wstr_view(WStr* str, uint start, uint length) {
	return (WStr) { str->data + start, length };
}