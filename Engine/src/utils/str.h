#pragma once

typedef struct Str {
	char* data;
	uint count;
} Str;

typedef struct WStr {
	wchar* data;
	uint count;
} WStr;

void str_create(Str* str, uint count);
void wstr_create(WStr* str, uint count);
void str_delete(Str* str);
void wstr_delete(WStr* str);

void str_resize(Str* str, uint new_count);
void wstr_resize(WStr* str, uint new_count);
void str_increase(Str* str, uint count);
void wstr_increase(WStr* str, uint count);
void str_zero(Str* str);
void wstr_zero(WStr* str);
void str_nl(Str* str);
void wstr_nl(WStr* str);

void str_catf(Str* str, const char* format, ...);
void wstr_catf(WStr* str, const wchar* format, ...);

void str_cat(Str* str, const char* data);
void wstr_cat(WStr* str, const wchar* data);
void str_catc(Str* str, const char* data, uint count);
void wstr_catc(WStr* str, const wchar* data, uint count);
void str_cats(Str* str, Str src);
void wstr_catws(WStr* str, WStr src);
void str_cati(Str* str, int data);
void wstr_cati(WStr* str, int data);

Str str_view(Str* str, uint start, uint length);
WStr wstr_view(WStr* str, uint start, uint length);