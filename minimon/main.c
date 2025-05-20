#include <stdio.h>
#include "core/collector.h"
#include "core/analyzer.h"
#include<unistd.h>


//here call the functions depending on the user's input arguments
int main(){

    MemoryUsage memory;

    MemStatsRaw mem;
    mem = get_mem_stats_raw();
    memory = get_memory_usage(mem);

    printf("Total RAM: %.1f GB\nUsed: %.1f GB\nSwap: %.1f GB / %.1f GB\n", memory.total_mem, 
        memory.used_mem, memory.swapped_mem, memory.total_swap);


   return 0;
}

void print_cpu_usage(){
    CPUStatsRaw cpu1;
    cpu1 = get_cpu_stats_raw();

    sleep(1);

    CPUStatsRaw cpu2;
    cpu2 = get_cpu_stats_raw();


    CPUUsage cpu_usage_percent = get_cpu_usage_percent(cpu1,cpu2);


    printf("CPU Usage: %.2f%% (Current user: %.2f%%, System: %.2f%%, Idle: %.2f%%)\n", cpu_usage_percent.usage, cpu_usage_percent.user, cpu_usage_percent.system, cpu_usage_percent.idle);
}