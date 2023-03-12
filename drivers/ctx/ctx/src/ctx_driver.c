#include "ctx_driver.h"

#include <string.h>

#define MAX_DRIVERS 8

typedef struct CtxDriverE {
	const char *name;
	int primary;
	CtxDriver driver;
} CtxDriverE;

static CtxDriverE s_drivers[MAX_DRIVERS];
static int s_drivers_cnt = 0;

void ctx_driver_add(const char *name, int primary, CtxDriver driver)
{
	s_drivers[s_drivers_cnt++] = (CtxDriverE){
		.name	 = name,
		.primary = primary,
		.driver	 = driver,
	};
}

const char *ctx_driver_get_primary()
{
	for (int i = 0; i < s_drivers_cnt; i++) {
		if (s_drivers[i].primary == 1) {
			return s_drivers[i].name;
		}
	}
	return NULL;
}

CtxDriver *ctx_driver_get(const char *name)
{
	for (int i = 0; i < s_drivers_cnt; i++) {
		if (strcmp(s_drivers[i].name, name) == 0) {
			return &s_drivers[i].driver;
		}
	}
	return NULL;
}

int ctx_driver_get_id(const char *name)
{
	for (int i = 0; i < s_drivers_cnt; i++) {
		if (strcmp(s_drivers[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

const char *ctx_driver_get_name(int id)
{
	if (id < 0 || id > s_drivers_cnt) {
		return NULL;
	}
	return s_drivers[id].name;
}

int ctx_driver_get_count()
{
	return s_drivers_cnt;
}

void ctx_driver_print()
{
	printf("ctx drivers:\n");
	for (int i = 0; i < s_drivers_cnt; i++) {
		printf("    %-10s %s\n", s_drivers[i].name, s_drivers[i].primary ? "(primary)" : "");
	}
}
