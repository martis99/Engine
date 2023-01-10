#include "utils_profiler.h"

#include "utils_none_types.h"

typedef struct Profiler {
	int temp;
} Profiler;

static Profiler *s_profiler;

void *utils_profiler_create()
{
	Profiler *profiler = malloc(sizeof(Profiler));
	if (profiler == NULL) {
		return NULL;
	}

	s_profiler = profiler;
	return profiler;
}

void utils_profiler_delete()
{
	free(s_profiler);
	s_profiler = NULL;
}

void enter_func(void *retAddress)
{
}

void exit_func(void *retAddress)
{
}

void utils_profiler_start(const char *file)
{
	printf("Started\n");
}

void utils_profiler_end()
{
	printf("Ended\n");
}
