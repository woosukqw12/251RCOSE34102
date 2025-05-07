#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "process.h"

#define NUM 5 //NumOfProcesses

process* Create_Process(void);
void print_process(process *p, int len);
process* copy_process(process *p);
process* copy_origin_process(process *p);



process* Create_Process(void){
    // int num_processes = NUM;
    // process* process_ = (process*) malloc(NUM * sizeof(process));
    process process_[NUM];
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

process* copy_origin_process(process *p){
    process* copied_process = (process*) malloc(NUM * sizeof(process));
    for (int i=0; i<NUM; i++){
        copied_process[i] = p[i];
    }
    return copied_process;
}

process* copy_process(process *p){
    process* copied_process = (process*) malloc(NUM * sizeof(process));
    for (int i=0; i<NUM; i++){
        copied_process[i] = p[i];
    }
    return copied_process;
}

//Job queue
int NoP_in_JQ; //rear
process* init_job_queue(process *p){
    process* job_queue = copy_origin_process(p);
    NoP_in_JQ = NUM;

    return job_queue;
}

process pop_from_job_queue(process* p, int target_idx){
    if (NoP_in_JQ <= 0) {
        process NullP;
        NullP.PID = -1;
        return NullP;
    }
    else{
        process poped_process = p[target_idx];

        for (int i=target_idx; i<NoP_in_JQ; i++){
            p[i] = p[i+1];
        }
        NoP_in_JQ--;

        return poped_process;
    }
}

//Ready queue
int NoP_in_RQ; //rear
process* init_ready_queue(void){
    process ready_queue[NUM];
    NoP_in_RQ = 0;

    return ready_queue;
}

process pop_from_ready_queue(process* p, int target_idx){
    if (NoP_in_RQ <= 0) {
        printf("ERROR: job queue is empty!!\n");
        process NullP;
        NullP.PID = -1;
        return NullP;
    }
    else{
        process poped_process = p[target_idx];

        for (int i=target_idx; i<NoP_in_RQ; i++){
            p[i] = p[i+1];
        }
        NoP_in_RQ--;

        return poped_process;
    }
}

process* insert_ready_queue(process* p, process insert){
    if (NoP_in_RQ < NUM){
        p[NoP_in_RQ] = insert;
        NoP_in_RQ++;

        return p;
    }
    else{
        printf("ERROR: ready queue is already full!!\n");
        return p;
    }
    
}















