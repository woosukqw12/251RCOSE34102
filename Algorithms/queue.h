#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "process.h"

#define NUM 10 //NumOfProcesses
#define DEBUG_MODE 0

process* Create_Process(void);
void print_process(process *p, int len);
process* copy_process(process *p);
process* copy_origin_process(process *p);



process* Create_Process(void){
    // int num_processes = NUM;
    process* process_ = (process*) malloc(NUM * sizeof(process));
    // process process_[NUM];
    for (int i=0; i<NUM; i++){
        process_[i].PID = i;
        process_[i].arrival_time = (rand()%10);
        process_[i].CPU_burst_time = (rand()%20)+1;
        process_[i].IO_burst_time = (rand()%10)+1;
        process_[i].IO_occur_left = rand()%3+1; //IO발생횟수 1~3회 랜덤
        process_[i].IO_request_time = (rand()%(process_[i].CPU_burst_time) + 1); //요청 시점 랜덤
        // if ( (rand()%100) < 60 ){
        //     process_[i].IO_request_time = (rand()%(process_[i].CPU_burst_time-1) + 1); // cpu burst내 범위에서 이 request_time만큼 process가 진행되면 IO_request를 받게 할 것임.
        // }
        // else{
        //     process_[i].IO_request_time = -1;
        // }
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
        if (DEBUG_MODE) printf("\t\tpop_from_job_queue\tPID: %d\n", poped_process.PID);
        
        return poped_process;
    }
}

//Ready queue
int NoP_in_RQ; //rear
process* init_ready_queue(void){
    process* ready_queue = (process*) malloc(NUM * sizeof(process));
    NoP_in_RQ = 0;

    return ready_queue;
}

process pop_from_ready_queue(process* p, int target_idx){
    if (NoP_in_RQ <= 0) {
        printf("ERROR: ready queue is empty!! pid:%d\n", p[target_idx].PID);
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
        

        if (DEBUG_MODE) printf("\t\tpop_from_ready_queue\tPID: %d\n", poped_process.PID);

        return poped_process;
    }
}

void insert_ready_queue(process* p, process insert, int mode){
    if (NoP_in_RQ < NUM){
        int idx = NoP_in_RQ;
        // if (DEBUG_MODE) printf("mode: %d\n", mode);
        switch (mode)
        {
        case 0:
            //non-preemptive, default=FCFS
            break;
        
        case 1:
            //Preemptive SJF
            for (int i=0; i<NoP_in_RQ; i++){
                if (DEBUG_MODE) printf("%d\t%d\n", p[i].CPU_burst_time, insert.CPU_burst_time);
                if (p[i].CPU_burst_time > insert.CPU_burst_time){
                    //insert보다 burst time이 커지는 p의 위치를 idx설정하여 그곳에 insert 수행
                    idx = i;
                    break;
                }
            }
            for (int i=NoP_in_RQ; i>idx; i--){
                p[i] = p[i-1];
            }
            break;

        case 2:
            //Preemptive Priority
            for (int i=0; i<NoP_in_RQ; i++){
                if (p[i].priority > insert.priority){
                    //insert보다 priority value가 커지는 p의 위치를 idx설정하여 그곳에 insert 수행
                    idx = i;
                    break;
                }
            }
            for (int i=NoP_in_RQ; i>idx; i--){
                p[i] = p[i-1];
            }
            break;

        case 3:
            //Non-preemptive SJF
            if (NoP_in_RQ > 1){
                // 선점 안하니까 len=0, 1일땐 그냥 뒤에 넣으면 됨
                for (int i=1; i<NoP_in_RQ; i++){
                    if (DEBUG_MODE) printf("%d\t%d\n", p[i].CPU_burst_time, insert.CPU_burst_time);
                    if (p[i].CPU_burst_time > insert.CPU_burst_time){
                        //insert보다 burst time이 커지는 p의 위치를 idx설정하여 그곳에 insert 수행
                        idx = i;
                        break;
                    }
                }
                for (int i=NoP_in_RQ; i>idx; i--){
                    p[i] = p[i-1];
                }
            }
            break;

        case 4:
            //Non-Preemptive Priority
            if (NoP_in_RQ > 1){
                // 선점 안하니까 len=0, 1일땐 그냥 뒤에 넣으면 됨
                for (int i=1; i<NoP_in_RQ; i++){
                    if (p[i].priority > insert.priority){
                        //insert보다 priority value가 커지는 p의 위치를 idx설정하여 그곳에 insert 수행
                        idx = i;
                        break;
                    }
                }
                for (int i=NoP_in_RQ; i>idx; i--){
                    p[i] = p[i-1];
                }
            }
            break;
        
        case 5:
            //Non-Preemptive HRN
            if (NoP_in_RQ > 1){
                // 선점 안하니까 len=0, 1일땐 그냥 뒤에 넣으면 됨
                // response ratio = (waiting time + cpu burst time)/cpu burst time
                for (int i=1; i<NoP_in_RQ; i++){
                    if ((p[i].waiting_time + p[i].CPU_burst_time)/p[i].CPU_burst_time\
                        < (insert.waiting_time + insert.CPU_burst_time)/insert.CPU_burst_time)
                    {
                        //insert보다 reponse ratio가 작아지는 p의 위치를 idx설정하여 그곳에 insert 수행
                        idx = i;
                        break;
                    }
                }
                for (int i=NoP_in_RQ; i>idx; i--){
                    p[i] = p[i-1];
                }
            }
            break;

        case 6:
            //Non-preemptive Custom. first: longest I/O burst, second: shortest cpu burst
            if (NoP_in_RQ > 1){
                // 선점 안하니까 len=0, 1일땐 그냥 뒤에 넣으면 됨
                for (int i=1; i<NoP_in_RQ; i++){
                    if (DEBUG_MODE) printf("%d\t%d\n", p[i].CPU_burst_time, insert.CPU_burst_time);
                    if (p[i].IO_request_time >= 0){
                        if (p[i].IO_burst_time < insert.IO_burst_time){
                            //insert보다 burst time이 커지는 p의 위치를 idx설정하여 그곳에 insert 수행
                            idx = i;
                            break;
                        }
                        else if ((p[i].IO_burst_time == insert.IO_burst_time) \
                                & (p[i].CPU_burst_time > insert.CPU_burst_time)){
                                idx = i;
                                break;
                            }
                    }
                    else{
                        // if p[i]가 I/O동작을 수행하지 않는다면,
                        idx = i;
                        break;
                    }
                    
                }
                for (int i=NoP_in_RQ; i>idx; i--){
                    p[i] = p[i-1];
                }
            }
            break;

        case 7:
            //Preemptive Custom. first: longest I/O burst, second: shortest cpu burst
            if (NoP_in_RQ > 1){
                for (int i=0; i<NoP_in_RQ; i++){
                    if (DEBUG_MODE) printf("%d\t%d\n", p[i].CPU_burst_time, insert.CPU_burst_time);
                    if (p[i].IO_request_time >= 0){
                        if (p[i].IO_burst_time < insert.IO_burst_time){
                            //insert보다 burst time이 커지는 p의 위치를 idx설정하여 그곳에 insert 수행
                            idx = i;
                            break;
                        }
                        else if ((p[i].IO_burst_time == insert.IO_burst_time) \
                                & (p[i].CPU_burst_time > insert.CPU_burst_time)){
                                idx = i;
                                break;
                            }
                    }
                    else{
                        // if p[i]가 I/O동작을 수행하지 않는다면,
                        idx = i;
                        break;
                    }
                    
                }
                for (int i=NoP_in_RQ; i>idx; i--){
                    p[i] = p[i-1];
                }
            }
            break;
        case 8:
            //Non-Preemptive HRN
            if (NoP_in_RQ > 1){
                // 선점 안하니까 len=0, 1일땐 그냥 뒤에 넣으면 됨
                // response ratio = (waiting time + cpu burst time)/cpu burst time
                for (int i=1; i<NoP_in_RQ; i++){
                    if ((p[i].waiting_time + p[i].CPU_burst_time)/p[i].CPU_burst_time\
                        < (insert.waiting_time + insert.CPU_burst_time)/insert.CPU_burst_time)
                    {
                        //insert보다 reponse ratio가 작아지는 p의 위치를 idx설정하여 그곳에 insert 수행
                        idx = i;
                        break;
                    }
                }
                for (int i=NoP_in_RQ; i>idx; i--){
                    p[i] = p[i-1];
                }
            }
            break;

        default:
            break;
        }
        p[idx] = insert;
        NoP_in_RQ++;
        
        if (DEBUG_MODE) printf("\t\tinsert_ready_queue\tPID: %d\n", insert.PID);
    }
    else{
        printf("ERROR: ready queue is already full!!\n");
    }
}
void lottery_drawing(process* p){
    //Non-preemptive lottery
    if (NoP_in_RQ > 0){
        int idx=0;
        int tatal_ticket = 0;
        int cur_ticket_sum= 0;
        for (int i=0; i<NoP_in_RQ; i++){
            tatal_ticket = tatal_ticket + p[i].ticket;
        }
        int win_ticket = (rand()%tatal_ticket)+1;
        for (int i=0; i<NoP_in_RQ; i++){
            cur_ticket_sum = cur_ticket_sum + p[i].ticket;
            if (cur_ticket_sum >= win_ticket){
                idx = i;
                printf("p%d\t",  p[idx].PID);
                break;
            }
        }
        if (idx != 0){
            process tmp = p[0];
            p[0] = p[idx];
            p[idx] = tmp;
        }
    }
}
void print_ready_queue(process* p){
    printf("ready queue[%d]:\t", NoP_in_RQ);
    for (int i=0; i<NoP_in_RQ; i++) printf("%d ", p[i].PID);
    printf("\n");
}

// void insert_ready_queue(process* p, process insert){
//     if (NoP_in_RQ < NUM){
//         p[NoP_in_RQ] = insert;
//         NoP_in_RQ++;
//         if (DEBUG_MODE) printf("\t\tinsert_ready_queue\tPID: %d\n", insert.PID);
//         // return p;
//     }
//     else{
//         printf("ERROR: ready queue is already full!!\n");
//         // return p;
//     }
    
// }

//Wait queue
int NoP_in_WQ; //rear
process* init_wait_queue(void){
    process* wait_queue = (process*) malloc(NUM * sizeof(process));
    NoP_in_WQ = 0;

    return wait_queue;
}

process pop_from_wait_queue(process* p, int target_idx){
    if (NoP_in_WQ <= 0) {
        printf("ERROR: wait queue is empty!!\n");
        process NullP;
        NullP.PID = -1;
        return NullP;
    }
    else{
        process poped_process = p[target_idx];

        for (int i=target_idx; i<NoP_in_WQ; i++){
            p[i] = p[i+1];
        }
        NoP_in_WQ--;

        if (DEBUG_MODE) printf("pop_from_wait_queue\tPID: %d\n", poped_process.PID);
        return poped_process;
    }
}

void insert_wait_queue(process* p, process insert, int mode){
    if (NoP_in_WQ < NUM){
        int idx;
        switch (mode)
        {
        case 0:
            //
            idx = NoP_in_WQ;
            break;
        
        case 1:
            //Preemptive SJF. 사용하지 않음
            for (int i=0; i<NoP_in_WQ; i++){
                if (p[i].CPU_burst_time > insert.CPU_burst_time){
                    idx = i;
                    break;
                }
            }
            for (int i=NoP_in_WQ; i>idx; i--){
                p[i] = p[i-1];
            }

        case 2:
            //Preemptive Priority. 사용하지 않음
            for (int i=0; i<NoP_in_WQ; i++){
                if (p[i].priority > insert.priority){
                    idx = i;
                    break;
                }
            }
            for (int i=NoP_in_WQ; i>idx; i--){
                p[i] = p[i-1];
            }

        default:
            break;
        }
        p[idx] = insert;
        NoP_in_WQ++;
        if (DEBUG_MODE) printf("\t\tinsert_wait_queue\tPID: %d\n", insert.PID);
    }
    else{
        printf("ERROR: wait queue is already full!!\n");
        // return p;
    }
    
}
void print_wait_queue(process* p){
    printf("wait  queue[%d]:\t", NoP_in_WQ);
    for (int i=0; i<NoP_in_WQ; i++) printf("%d(%d) ", p[i].PID, p[i].IO_burst_time);
    printf("\n");
}

//Terminated queue
int NoP_in_TQ; //rear
process* init_terminated_queue(void){
    process* terminated_queue = (process*) malloc(NUM * sizeof(process));
    NoP_in_TQ = 0;

    return terminated_queue;
}

void insert_terminated_queue(process* p, process insert){
    if (NoP_in_TQ < NUM){
        p[NoP_in_TQ] = insert;
        NoP_in_TQ++;
    }
    else{
        printf("ERROR: terminated queue is already full!!\n");
    }
    
}















