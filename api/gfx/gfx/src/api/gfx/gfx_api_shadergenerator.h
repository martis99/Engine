#pragma once
#include "gfx_api_types.h"

void ashadergenerator_generate(AShaderDesc desc, Str* vert, Str* frag);
const char* ashadergenerator_get_bnf();