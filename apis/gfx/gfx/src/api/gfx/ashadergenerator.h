#pragma once
#include "gfx_api_types.h"

void append(char* src, int* n, const char* text);
void append_i(char* src, int* n, int value);
void appends(char* src, int* n, uint count, ...);

const char* get_type_token(AType type);
void add_type(char* src, int* n, AType type);
void add_value(char* src, int* n, AValue value, bool semantic);
void add_values_block(char* src, int* n, ABufferDesc desc, bool semantic);
void add_struct(char* src, int* n, ABufferDesc desc);
void add_array(char* src, int* n, const char* name, uint nr);
void add_array_declaration(char* src, int* n, const char* type, const char* name, uint size, const char* casting);
void add_function_header(char* src, int* n, AType return_type, const char* name, AValue* args, uint args_size);
void add_switch(char* src, int* n, const char* var);
void add_case(char* src, int* n, int nr);

void ashadergenerator_generate(AShaderDesc desc, char* src_vert, char* src_frag, const char* vert, const char* frag);