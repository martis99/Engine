#include "ctx_driver.h"

#include <string.h>

#define MAX_DRIVERS 8

static const char *s_names[MAX_DRIVERS];
static CtxDriver s_drivers[MAX_DRIVERS];
static int s_drivers_cnt = 0;

void ctx_driver_add(const char *name, CtxDriver driver)
{
	s_names[s_drivers_cnt]	   = name;
	s_drivers[s_drivers_cnt++] = driver;
}

CtxDriver *ctx_driver_get(const char *name)
{
	for (int i = 0; i < s_drivers_cnt; i++) {
		if (strcmp(s_names[i], name) == 0) {
			return &s_drivers[i];
		}
	}
	return NULL;
}

int ctx_driver_get_id(const char *name)
{
	for (int i = 0; i < s_drivers_cnt; i++) {
		if (strcmp(s_names[i], name) == 0) {
			return i;
		}
	}
	return -1;
}

int ctx_driver_get_count()
{
	return s_drivers_cnt;
}

CtxDriver *ctx_driver_get_by_id(int id)
{
	if (id < 0 || id > s_drivers_cnt) {
		return NULL;
	}
	return &s_drivers[id];
}

const char *ctx_driver_get_name(int id)
{
	if (id < 0 || id > s_drivers_cnt) {
		return NULL;
	}
	return s_names[id];
}
