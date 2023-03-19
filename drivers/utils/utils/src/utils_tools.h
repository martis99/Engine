#pragma once

#define __USE_POSIX199309
#include <time.h>

void u_break();
int u_getrealtime(struct timespec *tv);
