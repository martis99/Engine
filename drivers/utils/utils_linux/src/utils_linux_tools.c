#include "utils_tools.h"

#include "utils_linux_types.h"

void u_break()
{
}

int u_getrealtime(struct timespec *tv)
{
	return clock_gettime(CLOCK_REALTIME, tv);
}
