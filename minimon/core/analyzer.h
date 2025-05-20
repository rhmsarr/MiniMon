#ifndef ANALYZER_H
#define ANALYZER_H
#include "collector.h"

typedef struct{
    float usage;
    float user;
    float system;
    float idle; 
} CPUUsage;

typedef struct
{
    float total_mem;
    float used_mem;
    float swapped_mem;
    float total_swap;
} MemoryUsage;

CPUUsage get_cpu_usage_percent(CPUStatsRaw a, CPUStatsRaw b);

MemoryUsage get_memory_usage(MemStatsRaw mem_stats);
#endif