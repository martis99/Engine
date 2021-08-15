#include "pch.h"
#ifdef SAPI_NONE
#include "utils/utils.h"

void debug_break() {
	__debugbreak();
}

#endif