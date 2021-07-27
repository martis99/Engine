#pragma once
#include "structs.h"

Uniform* uniform_create(Uniform* uniform, Shader* shader, const char* name, ADataType type, int count);
void uniform_delete(Uniform* uniform);

void uniform_set(Uniform* uniform, const void* data);
void* uniform_get(Uniform* uniform);
size_t uniform_size(Uniform* uniform);

void uniform_upload(Uniform* uniform);
void uniform_upload_vec1i(Uniform* uniform, int count, const void* data);
void uniform_upload_mat4f(Uniform* uniform, int count, const void* data);