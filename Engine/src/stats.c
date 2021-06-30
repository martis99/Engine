#include "pch.h"
#include "stats.h"

Stats* stats_create(Stats* stats) {
	stats->memory = 0;
	stats->draw_calls = 0;
	return stats;
}

void stats_delete(Stats* stats) {
	printf("memory: %i\n", (int)stats->memory);
}