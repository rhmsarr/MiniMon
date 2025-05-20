//sorting analysing?
#include <stdio.h>
#include "analyzer.h"




CPUUsage get_cpu_usage_percent(CPUStatsRaw a, CPUStatsRaw b) {
    CPUUsage usage = {0};

    unsigned long total_a = a.user + a.nice + a.system + a.idle +
                            a.iowait + a.irq + a.softirq + a.steal;
    unsigned long total_b = b.user + b.nice + b.system + b.idle +
                            b.iowait + b.irq + b.softirq + b.steal;

    unsigned long total_diff = total_b - total_a;
    if (total_diff == 0) return usage; 

    unsigned long user_diff = (b.user + b.nice) - (a.user + a.nice);
    unsigned long system_diff = (b.system + b.irq + b.softirq) - (a.system + a.irq + a.softirq);
    unsigned long idle_diff = (b.idle + b.iowait) - (a.idle + a.iowait);

    usage.user = (float)user_diff * 100 / total_diff;
    usage.system = (float)system_diff * 100 / total_diff;
    usage.idle = (float)idle_diff * 100 / total_diff;
    usage.usage = 100.0f - usage.idle;

    return usage;
}


MemoryUsage get_memory_usage(MemStatsRaw mem_stats){
    MemoryUsage mem;

    mem.total_mem = (float)mem_stats.total_mem / 1024.0 / 1024.0;  // kB → GB
    mem.used_mem = (float)(mem_stats.total_mem - mem_stats.available_mem) / 1024.0 / 1024.0;
    mem.total_swap = (float)mem_stats.total_swap / 1024.0 / 1024.0;
    mem.swapped_mem = (float)mem_stats.used_swap / 1024.0 / 1024.0;

    return mem;
}
