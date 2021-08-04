#pragma once
#include "api/atypes.h"

AMaterial* amaterial_create(ARenderer* renderer, AShader* shader, AValue* values, uint values_size);
void amaterial_delete(AMaterial* material);

void amaterial_set_value(AMaterial* material, uint index, const void* value);
void amaterial_upload(AMaterial* material, ARenderer* renderer);
void amaterial_bind(AMaterial* material, ARenderer* renderer, uint slot);