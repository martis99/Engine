#pragma once
#include "str_type.h"

#define STR_FLAG_SPACE 1 << 0
#define STR_FLAG_NEW_LINE 1 << 1
#define STR_FLAG_NULL 1 << 2
#define STR_FLAG_ALPHA 1 << 3
#define STR_FLAG_DIGIT 1 << 4
#define STR_FLAG_UNDERSCORE 1 << 5
#define STR_FLAG_FULL_STOP 1 << 6

void str_create(Str* str, uint count);
void str_create_cstrn(Str* str, const char* cstr, uint count);
void str_create_cstr(Str* str, const char* cstr, uint count);
void str_create_str(Str* str, Str* src);
void str_create_str_sub(Str* str, Str* src, uint start, uint count);
void str_create_default(Str* str);
void str_delete(Str* str);

int str_off_set(Str* str, uint offset);
int str_off_add(Str* str, uint count);
int str_off_reset(Str* str);

Str str_read(Str* str, int flag);
Str str_read_space(Str* str);
Str str_read_alpha_(Str* str);
Str str_read_alnum_(Str* str);
Str str_read_num(Str* str);
Str str_read_float(Str* str);

Str str_read_until_char(Str* str, char c);

char str_get_char(Str* str);
char str_read_char(Str* str, char c);

Str str_get_str(Str* str, Str* src);
Str str_read_str(Str* str, Str* src);

char* str_peek_at(Str* str, uint offset);
char* str_peek(Str* str);

void str_ensure_count(Str* str, uint count);

void str_set_char(Str* str, char c);

void str_add_char(Str* str, char c);
void str_add_cstr(Str* str, const char* cstr, size_t len);
void str_add_cstrf(Str* str, const char* format, ...);
void str_add_strn(Str* str, Str* src, uint count);
void str_add_str(Str* str, Str* src);
void str_add_int(Str* str, int val);
void str_add_nl(Str* str);

void str_truncate(Str* str, uint offset);
void str_clear(Str* str);

int str_cmp_char(Str* str, char c);
int str_cmp_cstr(Str* str, const char* cstr, size_t len);
int str_cmp_str(Str* str, Str* src);
int str_cmp_str_offset(Str* str, Str* src);

int str_eol(Str* str);
int str_eos(Str* str);

int str_len(Str* str);

void str_print(Str* str);


void wstr_create(WStr* str, uint count);
void wstr_delete(WStr* str);
void wstr_resize(WStr* str, uint new_count);
void wstr_increase(WStr* str, uint count);
void wstr_zero(WStr* str);
void wstr_nl(WStr* str);
void wstr_catf(WStr* str, const wchar* format, ...);
void wstr_cat(WStr* str, const wchar* data);
void wstr_catch(WStr* str, wchar data);
void wstr_catc(WStr* str, const wchar* data, uint count);
void wstr_catws(WStr* str, WStr src);
void wstr_cati(WStr* str, int data);
void wstr_cpy(WStr* str, wchar* dst);
int wstr_cmp(WStr* str, const wchar* data);
WStr wstr_view(WStr* str, uint start, uint length);