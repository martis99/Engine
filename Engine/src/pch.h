#pragma once
typedef unsigned char byte;
typedef unsigned char bool;
typedef unsigned short wchar;
typedef unsigned int uint;

#define false 0
#define true 1

#define A_FAIL 0
#define A_SUCCESS 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string.h>
#include <math.h>

#include "log.h"
#include "mem.h"

#include "math/maths.h"

#include "utils/dictionary.h"
#include "utils/stack.h"
#include "utils/array.h"

#include "ecs/ecs.h"