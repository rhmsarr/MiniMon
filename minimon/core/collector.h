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

typedef struct {
    char mount_point[128];
    unsigned long total_kb;
    unsigned long used_kb;
    unsigned long free_kb;
    float used_percent;
} DiskUsageStatsRaw;

typedef struct {
    char device[32];
    unsigned long reads;
    unsigned long writes;
} DiskIOStats;

typedef struct {
    int PID;
    char name[50];
    char state;
    float memory;



} ProcessStatsRaw;




CPUStatsRaw get_cpu_stats_raw();

MemStatsRaw get_mem_stats_raw();

DiskUsageStatsRaw get_disk_usage(const char* path);

DiskIOStats get_disk_io_stats(const char* device_name);

#endif