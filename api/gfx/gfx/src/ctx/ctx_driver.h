#pragma once
#include "ctx_types.h"

#include "driver.h"

void ctx_driver_add(const char *name, CtxDriver driver);
CtxDriver *ctx_driver_get(const char *name);
int ctx_driver_get_id(const char *name);
int ctx_driver_get_count();
CtxDriver *ctx_driver_get_by_id(int id);
const char *ctx_driver_get_name(int id);

#define CTX_DRIVER(_name, _driver)               \
	DRIVER(ctx_drtiver_##_name)              \
	{                                        \
		ctx_driver_add(#_name, _driver); \
	}
