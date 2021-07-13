#pragma once
#include "ashader.h"
#include "adatatype.h"

typedef struct AUniform AUniform;

AUniform* auniform_create(AShader* shader, const char* name, ADataType type, int count);
void auniform_delete(AUniform* unfiform);

void auniform_set(AUniform* uniform, const void* data);
void* auniform_get(AUniform* uniform);
size_t auniform_size(AUniform* uniform);

void auniform_upload(AUniform* uniform);
void auniform_upload_vec1i(AUniform* uniform, int count, const void* data);
void auniform_upload_mat4f(AUniform* uniform, int count, const void* data);