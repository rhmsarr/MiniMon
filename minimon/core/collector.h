#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <stdio.h>

#define MAX_SIZE 100


typedef struct {
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;
} CPUStatsRaw;

typedef struct{
    float total_mem;
    float available_mem;
    float total_swap;
    float used_swap;
} MemStatsRaw;

CPUStatsRaw get_cpu_stats_raw();

MemStatsRaw get_mem_stats_raw();

#endif