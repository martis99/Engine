#include "pch.h"
#ifdef SAPI_WINDOWS
#include "utils/utils.h"

#include <Windows.h>

void debug_break() {
	DebugBreak();
}

#endif