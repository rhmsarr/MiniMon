#include <stdio.h>
#include <string.h>
#include<stdlib.h>

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