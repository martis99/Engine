#pragma once
#include "ctx_types.h"

#include "driver.h"

void ctx_driver_add(const char *name, int primary, CtxDriver driver);
const char *ctx_driver_get_primary();
CtxDriver *ctx_driver_get(const char *name);
int ctx_driver_get_id(const char *name);
const char *ctx_driver_get_name(int id);

int ctx_driver_get_count();

void ctx_driver_print();

#define CTX_DRIVER(_name, _primary, _driver)               \
	DRIVER(ctx_driver_##_name)                         \
	{                                                  \
		ctx_driver_add(#_name, _primary, _driver); \
	}
