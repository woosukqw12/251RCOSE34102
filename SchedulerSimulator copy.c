#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "Algorithms.h"
#include "Algorithms/process.h"

#define NUMofAlgorithms 6
#define NUM 5

//global assign 
process* origin;
char *algorithms[NUMofAlgorithms] = {"FCFS", "SJF", "Priority", "RR", "PreemptiveSJF", "PreemptivePriority"};

//function assign
process* Create_Process(void);
void print_process(process *p, int len);
process* copy_process(process *p);
void Schedule(void);


//Job queue
process* Create_Process(void){
    // int num_processes = NUM;
    process* process_ = (process*) malloc(NUM * sizeof(process));
    for (int i=0; i<NUM; i++){
        process_[i].PID = i;
        process_[i].arrival_time = (rand()%2);
        process_[i].CPU_burst_time = (rand()%20)+1;
        process_[i].IO_burst_time = (rand()%11)+1;
        // process_[i].IO_request_time = 
        process_[i].priority = (rand()%10)+1;

        process_[i].waiting_time = 0;
        process_[i].turnaround_time = 0;
        process_[i].terminated_time = 0;
    }

    // print_process(process_, NUM);
    return process_;
}

void print_process(process *p, int len){
    for (int i=0; i<len; i++){
        printf("[process %d]: (arrival time: %d /cpu burst: %d /IO burst %d/priority: %d)\n", \
            p[i].PID, p[i].arrival_time, p[i].CPU_burst_time, p[i].IO_burst_time, p[i].priority);
    }
}

process* copy_process(process *p){
    process* copied_process = (process*) malloc(NUM * sizeof(process));
    for (int i=0; i<NUM; i++){
        copied_process[i] = p[i];
    }
    return copied_process;
}

void Schedule(void){
    process* origin = Create_Process();
    process* p1 = copy_process(origin);
    process* p2 = copy_process(origin);
    process* p3 = copy_process(origin);

    // printf("List of scheduling algorithms\n");
    // for (int i=0; i<NUMofAlgorithms; i++) printf("[%d]. %s\n", i+1, algorithms[i]);
    // for (int i=0; i<NUM; i++) {
    //     printf("PID: %d, arrival_time: %d, burst_time: %d\t\t", origin[i].PID,  origin[i].arrival_time, origin[i].CPU_burst_time);
    //     printf("PID: %d, arrival_time: %d, burst_time: %d\n", p1[i].PID,  p1[i].arrival_time, p1[i].CPU_burst_time);
    // }

    
    FCFS(p1, NUM);
    SJF(p2, NUM);
    Priority(p3, NUM);
    
}



int main(void){
    Schedule();
}