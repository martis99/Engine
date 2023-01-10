#include "utils_profiler.h"

#include "utils_win_types.h"

#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

typedef struct Profiler {
	HANDLE process;
	PSYMBOL_INFO info;
	LONGLONG *fns;
	int fn_count;
	LONGLONG frequency;
	FILE *file;
	int first;
} Profiler;

static Profiler *s_profiler;

void *utils_profiler_create()
{
	Profiler *profiler = malloc(sizeof(Profiler));
	if (profiler == NULL) {
		return NULL;
	}
	profiler->process = GetCurrentProcess();

	SymInitialize(profiler->process, NULL, TRUE);
	profiler->info = malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);
	if (profiler->info == NULL) {
		return NULL;
	}
	profiler->info->MaxNameLen   = MAX_SYM_NAME;
	profiler->info->SizeOfStruct = sizeof(SYMBOL_INFO);

	profiler->fns	   = malloc(sizeof(LONGLONG) * 40);
	profiler->fn_count = 0;

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	profiler->frequency = freq.QuadPart;

	profiler->file	= NULL;
	profiler->first = 1;
	s_profiler	= profiler;
	return profiler;
}

void utils_profiler_delete()
{
	free(s_profiler->fns);
	free(s_profiler->info);
	SymCleanup(s_profiler->process);
	free(s_profiler);
	s_profiler = NULL;
}

void enter_func(void *retAddress)
{
	if (s_profiler == NULL || s_profiler->file == NULL) {
		return;
	}

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	s_profiler->fns[s_profiler->fn_count] = (time.QuadPart * 1000000) / s_profiler->frequency;
	if (s_profiler->fn_count > 0 && s_profiler->fns[s_profiler->fn_count] <= s_profiler->fns[s_profiler->fn_count - 1]) {
		s_profiler->fns[s_profiler->fn_count] = s_profiler->fns[s_profiler->fn_count - 1] + 1;
	}
	s_profiler->fn_count++;
}

void exit_func(void *retAddress)
{
	if (s_profiler == NULL || s_profiler->file == NULL || s_profiler->fn_count == 0) {
		return;
	}

	DWORD64 disp;
	SymFromAddr(s_profiler->process, (DWORD64)retAddress, &disp, s_profiler->info);

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	s_profiler->fn_count--;

	LONGLONG end = (time.QuadPart * 1000000) / s_profiler->frequency;
	LONGLONG mic = end - s_profiler->fns[s_profiler->fn_count];

	if (s_profiler->first == 0) {
		fprintf(s_profiler->file, ",\n");
	}
	s_profiler->first = 0;

	fprintf(s_profiler->file,
		"{"
		"\"cat\":\"function\","
		"\"dur\":%llu,"
		"\"name\":\"%s\","
		"\"ph\":\"X\","
		"\"pid\":0,"
		"\"tid\":0,"
		"\"ts\":%llu"
		"}",
		mic, s_profiler->info->Name, s_profiler->fns[s_profiler->fn_count]);
}

void utils_profiler_start(const char *file)
{
	s_profiler->fn_count = 0;
	s_profiler->first    = 1;

	fopen_s(&s_profiler->file, file, "w");
	if (s_profiler->file == NULL) {
		return;
	}
	fprintf(s_profiler->file, "[\n");
	printf("Started\n");
}

void utils_profiler_end()
{
	fprintf(s_profiler->file, "]\n");
	fclose(s_profiler->file);
	s_profiler->file = NULL;
	printf("Ended\n");
}
