#include "pch.h"
#include "ashadergenerator.h"

void append(char* src, int* n, const char* text) {
	strcpy_s(src + *n, strlen(text) + 1, text);
	(*n) += (int)strlen(text);
}

void append_i(char* src, int* n, int value) {
	char str_size[3];
	_itoa_s(value, str_size, 3, 10);
	append(src, n, str_size);
}

void appends(char* src, int* n, uint count, ...) {
	va_list ap;
	va_start(ap, count);
	for (uint i = 0; i < count; i++) {
		append(src, n, va_arg(ap, const char*));
	}
	va_end(ap);
}

void add_type(char* src, int* n, AType type) {
	append(src, n, get_type_token(type));
}

void add_value(char* src, int* n, AValue value, bool semantic) {
	add_type(src, n, value.type);
	appends(src, n, 2, " ", value.name);
	if (semantic == 1) {
		appends(src, n, 2, " : ", value.name);
	}
}

void add_values_block(char* src, int* n, ABufferDesc desc, bool semantic) {
	append(src, n, "{\n");
	uint values_count = desc.values_size / sizeof(AValue);
	for (uint i = 0; i < values_count; i++) {
		append(src, n, "	");
		add_value(src, n, desc.values[i], semantic);
		append(src, n, ";\n");
	}
	append(src, n, "};\n");
}

void add_struct(char* src, int* n, ABufferDesc desc) {
	appends(src, n, 3, "struct ", desc.name, " ");
	add_values_block(src, n, desc, 1);
}

void add_array(char* src, int* n, const char* name, uint nr) {
	appends(src, n, 2, name, "[");
	append_i(src, n, nr);
	append(src, n, "]");
}

void add_array_declaration(char* src, int* n, const char* type, const char* name, uint size, const char* casting) {
	append(src, n, type);
	if (casting != NULL) {
		appends(src, n, 3, "<", casting, ">");
	}
	append(src, n, " ");
	add_array(src, n, name, size);
	append(src, n, ";\n");
}

void add_function_header(char* src, int* n, AType return_type, const char* name, AValue* args, uint args_size) {
	add_type(src, n, return_type);
	appends(src, n, 3, " ", name, "(");
	uint args_count = args_size / sizeof(AValue);
	for (uint i = 0; i < args_count; i++) {
		add_value(src, n, args[i], 0);
		if (i < args_count - 1) {
			append(src, n, ", ");
		}
	}
	append(src, n, ")");
}

void add_switch(char* src, int* n, const char* var) {
	appends(src, n, 3, "switch (", var, ")");
}

void add_case(char* src, int* n, int nr) {
	append(src, n, "case ");
	append_i(src, n, nr);
	append(src, n, ": ");
}