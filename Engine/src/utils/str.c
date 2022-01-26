#include "str.h"

#include "mem.h"

#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void str_create(Str* str, uint count) {
	str->data = m_malloc(count * sizeof(char));
	str->count = count;
	str->offset = 0;
	str->view = 0;
}

void str_create_cstrn(Str* str, const char* cstr, uint count) {
	str->data = cstr;
	str->count = count;
	str->offset = 0;
	str->view = 1;
}

void str_create_cstr(Str* str, const char* cstr, uint count) {
	str_create_cstrn(str, cstr, count == 0 ? strlen(cstr) : count);
}

void str_create_str(Str* str, Str* src) {
	str_create_cstrn(str, src->data, src->count);
}

void str_create_str_sub(Str* str, Str* src, uint start, uint count) {
	str_create_cstrn(str, str_peek_at(src, start), count);
}

void str_create_default(Str* str) {
	str->data = NULL;
	str->count = 0;
	str->offset = 0;
	str->view = 0;
}

void str_delete(Str* str) {
	if (str->view == 1) {
		return;
	}

	m_free(str->data, str->count * sizeof(char));
}

int str_off_set(Str* str, uint offset) {
	str->offset = offset;
	return str->offset;
}

int str_off_add(Str* str, uint count) {
	return str_off_set(str, str->offset + count);
}

int str_off_reset(Str* str) {
	return str_off_set(str, 0);
}

char* str_peek_at(Str* str, uint offset) {
	if (offset > str->count) {
		return NULL;
	}
	return &str->data[offset];
}

char* str_peek(Str* str) {
	return str_peek_at(str, str->offset);
}

void str_set_char(Str* str, char c) {
	str->data[str->offset] = c;
}

char str_get_char(Str* str) {
	return str->data[str->offset];
}

Str str_get_str(Str* str, Str* src) {
	Str result;

	if (str_cmp_str_offset(str, src)) {
		str_create_cstr(&result, str_peek(str), src->count);
	} else {
		str_create_default(&result);

	}
	return result;
}

Str str_read(Str* str, int flag) {
	int space = flag & STR_FLAG_SPACE;
	int new_line = flag & STR_FLAG_NEW_LINE;
	int null = flag & STR_FLAG_NULL;
	int alpha = flag & STR_FLAG_ALPHA;
	int digit = flag & STR_FLAG_DIGIT;
	int underscore = flag & STR_FLAG_UNDERSCORE;
	int full_stop = flag & STR_FLAG_FULL_STOP;

	int count = 0;
	while (str->offset + count < str->count && (
		(space && str->data[str->offset + count] == ' ') ||
		(new_line && str->data[str->offset + count] == '\n') ||
		(null && str->data[str->offset + count] == '\0') ||
		(alpha && isalpha(str->data[str->offset + count])) ||
		(digit && isdigit(str->data[str->offset + count])) ||
		(underscore && str->data[str->offset + count] == '_') ||
		(full_stop && str->data[str->offset + count] == '.')
		)) {
		count++;
	}

	Str result;
	str_create_cstrn(&result, str_peek(str), count);
	str_off_add(str, count);
	return result;
}

Str str_read_space(Str* str) {
	return str_read(str, STR_FLAG_SPACE);
}

Str str_read_alpha_(Str* str) {
	return str_read(str, STR_FLAG_ALPHA | STR_FLAG_UNDERSCORE);
}

Str str_read_alnum_(Str* str) {
	return str_read(str, STR_FLAG_ALPHA | STR_FLAG_DIGIT | STR_FLAG_UNDERSCORE);
}

Str str_read_num(Str* str) {
	return str_read(str, STR_FLAG_DIGIT);
}

Str str_read_float(Str* str) {
	Str result;
	int offset = str->offset;

	Str l = str_read_num(str);
	if (str_read_char(str, '.') != '.') {
		str->offset = offset;
		str_create_default(&result);
		return result;
	}
	Str r = str_read_num(str);

	str_create_cstrn(&result, str_peek_at(str, offset), l.count + 1 + r.count);
	return result;
}

Str str_read_until_char(Str* str, char c) {
	int count = 0;
	while (str->data[str->offset + count] != c) {
		count++;
	}

	Str result;
	str_create_cstr(&result, str_peek(str), count);
	str_off_add(str, count);
	return result;
}

char str_read_char(Str* str, char c) {
	if (str_get_char(str) != c) {
		return 0;
	}
	str_off_add(str, 1);
	return c;
}

Str str_read_str(Str* str, Str* src) {
	Str result = str_get_str(str, src);
	str_off_add(str, result.count);
	return result;
}

void str_ensure_count(Str* str, uint count) {
	if (str->view == 1) {
		return;
	}

	if (str->count >= str->offset + count + 1) {
		return;
	}

	int old_count = str->count;

	while (str->count < str->offset + count + 1) {
		str->count <<= 1;
	}

	char* data = m_realloc(str->data, str->count * sizeof(char), old_count * sizeof(char));
	if (data != NULL) {
		str->data = data;
	}
}

void str_add_char(Str* str, char c) {
	if (str->view == 1) {
		return;
	}

	str_ensure_count(str, 1);
	str_set_char(str, c);
	str_off_add(str, 1);
	str_set_char(str, '\0');
}

void str_add_cstr(Str* str, const char* cstr, size_t len) {
	if (str->view == 1 || *cstr == '\0') {
		return;
	}

	if (len == 0) {
		len = strlen(cstr);
	}

	str_ensure_count(str, (uint)len);
	memcpy(str_peek(str), cstr, len * sizeof(char));
	str_off_add(str, (uint)len);
	str_set_char(str, '\0');
}

void str_add_cstrf(Str* str, const char* format, ...) {
	if (str->view == 1) {
		return;
	}

	static char buffer[2048];
	va_list args;
	va_start(args, format);
	int count = vsprintf_s(buffer, sizeof(buffer) / sizeof(char), format, args);
	va_end(args);
	str_add_cstr(str, buffer, count);
}

void str_add_strn(Str* str, Str* src, uint count) {
	str_add_cstr(str, str_peek(src), count);
}

void str_add_str(Str* str, Str* src) {
	str_add_strn(str, src, str_len(src));
}

void str_add_int(Str* str, int val) {
	if (str->view == 1) {
		return;
	}

	char buffer[12];
	_itoa_s(val, buffer, sizeof(buffer) / sizeof(char), 10);
	str_add_cstr(str, buffer, 0);
}

void str_add_nl(Str* str) {
	str_add_char(str, '\n');
}

void str_truncate(Str* str, uint offset) {
	if (offset > str->offset) {
		return;
	}

	str->offset = offset;
	str_set_char(str, '\0');
}

void str_clear(Str* str) {
	str_truncate(str, 0);
}

int str_cmp_char(Str* str, char c) {
	if (str->count != 1) {
		return 0;
	}
	return str->data[0] == c;
}

int str_cmp_cstr(Str* str, const char* cstr, size_t len) {
	if (len == 0) {
		len = strlen(cstr);
	}
	return str->count == len && strncmp(str->data, cstr, len) == 0;
}

int str_cmp_str(Str* str, Str* src) {
	return str_cmp_cstr(str, src->data, src->count);
}

static int str_min(int a, int b) {
	return a < b ? a : b;
}

int str_cmp_str_offset(Str* str, Str* src) {
	return str_cmp_cstr(src, str_peek(str), (size_t)str_min(str->count - str->offset, src->count));
}

int str_eol(Str* str) {
	return str->offset >= str->count || str_get_char(str) == '\n' || str_get_char(str) == '\0';
}

int str_eos(Str* str) {
	return str->offset >= str->count || str_get_char(str) == '\0';
}

int str_len(Str* str) {
	return strnlen_s(str_peek(str), (size_t)(str->count - str->offset));
}

void str_print(Str* str) {
	printf("%.*s", str->count, str->data);
}





void wstr_create(WStr* str, uint count) {
	str->data = malloc(count * sizeof(wchar));
	str->count = count;
}

void wstr_delete(WStr* str) {
	free(str->data);
}

void wstr_resize(WStr* str, uint new_count) {
	if (new_count > str->count) {
		void* data = realloc(str->data, new_count * sizeof(wchar));
		if (data != NULL) {
			str->data = data;
		}
		str->count = new_count;
	}
}

void wstr_increase(WStr* str, uint count) {
	wstr_resize(str, ((uint)wcslen(str->data) + count) * (uint)sizeof(wchar));
}

void wstr_zero(WStr* str) {
	memset(str->data, 0, str->count * sizeof(wchar));
}

void wstr_nl(WStr* str) {
	wstr_cat(str, L"\n");
}

void wstr_catf(WStr* str, const wchar* format, ...) {
	va_list args;
	va_start(args, format);
	vswprintf_s(str->data + wcslen(str->data), str->count - wcslen(str->data), format, args);
	va_end(args);
}

void wstr_cat(WStr* str, const wchar* data) {
	wstr_catc(str, data, (uint)wcslen(data));
}

void wstr_catch(WStr* str, wchar data) {
	wstr_increase(str, 1);
	str->data[wcslen(str->data)] = data;
}

void wstr_catc(WStr* str, const wchar* data, uint count) {
	wstr_increase(str, count);
	wcsncat_s(str->data, str->count, data, count);
}

void wstr_catws(WStr* str, WStr src) {
	wstr_catc(str, src.data, src.count);
}

void wstr_cati(WStr* str, int value) {
	wchar str_val[10] = { 0 };
	_itow_s(value, str_val, 10, 10);
	wstr_cat(str, str_val);
}

void wstr_cpy(WStr* str, wchar* dst) {
	memcpy(dst, str->data, str->count * sizeof(wchar));
}

int wstr_cmp(WStr* str, const wchar* data) {
	return wcscmp(str->data, data);
}

WStr wstr_view(WStr* str, uint start, uint length) {
	return (WStr) { str->data + start, length };
}