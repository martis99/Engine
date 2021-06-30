#pragma once

typedef struct Stats {
	size_t memory;
	int draw_calls;
} Stats;

Stats* stats_create(Stats* stats);
void stats_delete(Stats* stats);