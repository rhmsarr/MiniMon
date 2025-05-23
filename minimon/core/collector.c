#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <ctype.h>


//here, all the functions to do the analysis
#include "collector.h"

CPUStatsRaw get_cpu_stats_raw(){
    FILE *fileptr;
    char stat_file[] = "/proc/stat";
    
    fileptr = fopen(stat_file,"r" );
    
    //checking the file opens properly
    if(fileptr == NULL)
    {
        printf("The %s file could not be opened. The program will terminate.", stat_file);
        CPUStatsRaw empty = {0};
       return empty;

    }

    //getting the header line of the stat file, containing all the cpu information
    char cpu_info[MAX_SIZE];
    fgets(cpu_info, sizeof(cpu_info), fileptr);

    CPUStatsRaw cpu_1;
    //parsing the line
    //column 0 is the CPU name (cpu; then cpu0 up to cpuN), not needed
    char* token = strtok((cpu_info), " ");

    //column 1 is the time spent running user processes
    token = strtok(NULL, " ");
    cpu_1.user = atof(token);

    //column 2 is the time spent running user niced processes
    token = strtok(NULL, " ");
    cpu_1.nice = atof(token);

    //column 3 is the time spent running kernel mode processes
    token = strtok(NULL, " ");
    cpu_1.system = atof(token);

    //column 4 is the time spent idle
    token = strtok(NULL, " ");
    cpu_1.idle = atof(token);

    //column 5 is the time spent waiting on I/O
    token = strtok(NULL, " ");
    cpu_1.iowait = atof(token);

    //column 6 is the time spent running user niced processes
    token = strtok(NULL, " ");
    cpu_1.irq = atof(token);

    //column 7 is the time spent by hardware interrupts
    token = strtok(NULL, " ");
    cpu_1.softirq = atof(token);

    //column 8 is the time stolen by VM's
    token = strtok(NULL, " ");
    cpu_1.steal = atof(token);



    return cpu_1;
}

MemStatsRaw get_mem_stats_raw(){
    FILE *fileptr;
    char stat_file[] = "/proc/meminfo";
    
    fileptr = fopen(stat_file,"r" );
    
    //checking the file opens properly
    if(fileptr == NULL)
    {
        printf("The %s file could not be opened. The program will terminate.", stat_file);
        MemStatsRaw empty = {0};
        return empty;

    }
    char line[MAX_SIZE];
    MemStatsRaw memory_stats;

    fgets(line, sizeof(line), fileptr);
    //first token is the column header, not needed
    char* token = strtok((line), " \n");
    token = strtok(NULL, " \n");
    memory_stats.total_mem = atof(token);
    //moving on to the next line, second token
    fgets(line, sizeof(line), fileptr);
    fgets(line, sizeof(line), fileptr);
    token = strtok(line, " \n");
    token = strtok(NULL, " \n");
    memory_stats.available_mem = atof(token);


    for(int i = 0; i<12; i++)
        fgets(line, sizeof(line), fileptr);
    

    token = strtok(line, " \n");
    token = strtok(NULL, " \n");
    memory_stats.total_swap = atof(token);

    fgets(line, sizeof(line), fileptr);
    token = strtok(line, " \n");
    token = strtok(NULL, " \n");
    memory_stats.used_swap = atof(token);

    return memory_stats;
    
}



DiskUsageStatsRaw get_disk_usage(const char* path) {
    struct statvfs fs;
    DiskUsageStatsRaw usage = {0};

    if (statvfs(path, &fs) != 0) {
        perror("statvfs failed");
        return usage;
    }

    unsigned long total = fs.f_blocks * fs.f_frsize / 1024; 
    unsigned long free = fs.f_bfree * fs.f_frsize / 1024;   
    unsigned long available = fs.f_bavail * fs.f_frsize / 1024;
    unsigned long used = total - free;

    usage.total_kb = total;
    usage.free_kb = available;
    usage.used_kb = used;
    usage.used_percent = 100.0f * used / total;
    strncpy(usage.mount_point, path, sizeof(usage.mount_point) - 1);

    return usage;
}

DiskIOStats get_disk_io_stats(const char* device_name) {
    FILE* fp = fopen("/proc/diskstats", "r");
    DiskIOStats stats = {0};

    if (!fp) {
        perror("Failed to open /proc/diskstats");
        return stats;
    }

    char dev[32];
    unsigned long reads, read_sec, writes, write_sec;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        int major, minor;
        if (sscanf(line, "%d %d %31s %lu %*u %*u %*u %lu %*u %*u %*u",
                   &major, &minor, dev, &reads, &writes) == 5) {
            if (strcmp(dev, device_name) == 0) {
                strncpy(stats.device, dev, sizeof(stats.device));
                stats.reads = reads;
                stats.writes = writes;
                break;
            }
        }
    }

    fclose(fp);
    return stats;
}

int get_process_list(ProcessStatsRaw *plist, int max) {
    DIR *dir = opendir("/proc");
    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < max) {
        if (!isdigit(entry->d_name[0])) continue;

        char status_path[256];
        snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);

        FILE *fp = fopen(status_path, "r");
        if (!fp) continue;

        ProcessStatsRaw proc = {0};
        proc.PID = atoi(entry->d_name);
        char line[256];

        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "Name:", 5) == 0) {
                sscanf(line + 5, "%s", proc.name);
            } else if (strncmp(line, "State:", 6) == 0) {
                char state_code;
                sscanf(line + 6, " %c", &state_code);
                switch (state_code) {
                    case 'R': strcpy(proc.state, "Running"); break;
                    case 'S': strcpy(proc.state, "Sleeping"); break;
                    case 'D': strcpy(proc.state, "Waiting"); break;
                    case 'Z': strcpy(proc.state, "Zombie"); break;
                    case 'T': strcpy(proc.state, "Stopped"); break;
                    default: strcpy(proc.state, "Unknown"); break;
                }
            } else if (strncmp(line, "VmRSS:", 6) == 0) {
                sscanf(line + 6, "%lu", &proc.memory);
            }
        }

        fclose(fp);
        plist[count++] = proc;
    }
}

