#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <stdio.h>

#define MAX_SIZE 100

typedef struct{
    float usage;
    float user;
    float system;
    float idle; 
} CPUUsage;

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


CPUStatsRaw get_cpu_stats_raw();

#endif