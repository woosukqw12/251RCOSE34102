#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "Algorithms.h"


#define NUMofAlgorithms 6


//global assign 

char *algorithms[NUMofAlgorithms] = {"FCFS", "SJF", "Priority", "RR", "PreemptiveSJF", "PreemptivePriority"};

//function assign
// void Schedule(void);

void Schedule(void){
    process* origin = Create_Process();
    process* p1 = copy_origin_process(origin);
    process* p2 = copy_origin_process(origin);
    process* p3 = copy_origin_process(origin);
    process* p4 = copy_origin_process(origin);
    process* p5 = copy_origin_process(origin);
    process* p6 = copy_origin_process(origin);

    // printf("List of scheduling algorithms\n");
    // for (int i=0; i<NUMofAlgorithms; i++) printf("[%d]. %s\n", i+1, algorithms[i]);
    // for (int i=0; i<NUM; i++) {
    //     printf("PID: %d, arrival_time: %d, burst_time: %d\t\t", origin[i].PID,  origin[i].arrival_time, origin[i].CPU_burst_time);
    //     printf("PID: %d, arrival_time: %d, burst_time: %d\n", p1[i].PID,  p1[i].arrival_time, p1[i].CPU_burst_time);
    // }

    
    // FCFS(p1, NUM);
    // SJF(p2, NUM);
    // Priority(p3, NUM);
    // RR(p4, NUM);
    // PreemptiveSJF(p5, NUM);
    PreemptivePriority(p6, NUM);
    
}



int main(void){
    Schedule();
}