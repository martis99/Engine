#pragma once
#include "gfx_types.h"

#include "driver.h"

void gfx_driver_add(const char *name, GfxDriver driver);
GfxDriver *gfx_driver_get(const char *name);
int gfx_driver_get_id(const char *name);
int gfx_driver_get_count();
GfxDriver *gfx_driver_get_by_id(int id);
const char *gfx_driver_get_name(int id);

#define GFX_DRIVER(_name, _driver)               \
	DRIVER(gfx_driver_##_name)               \
	{                                        \
		gfx_driver_add(#_name, _driver); \
	}
