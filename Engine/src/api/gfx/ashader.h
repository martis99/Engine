#pragma once

typedef struct AShader AShader;

AShader* ashader_create(const char* src_vert, const char* src_frag);
void ashader_delete(AShader* shader);

void ashader_bind(AShader* shader);