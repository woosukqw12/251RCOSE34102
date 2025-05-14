#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "Algorithms.h"


#define NUMofAlgorithms 9


//global assign 

char *algorithms[NUMofAlgorithms] = {"FCFS", "Non-preempitve SJF", "Non-preempitve Priority", "RR", \
    "Preemptive SJF", "Preemptive Priority", "HRN", "Non-preempitve Custom", "preempitve Custom"};
evaluation_result* result_array;
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
    process* p7 = copy_origin_process(origin);
    process* p8 = copy_origin_process(origin);
    process* p9 = copy_origin_process(origin);
    
    result_array[0] = FCFS(p1, NUM);
    result_array[1] = SJF(p2, NUM);
    result_array[2] = Priority(p3, NUM);
    result_array[3] = RR(p4, NUM);
    result_array[4] = PreemptiveSJF(p5, NUM);
    result_array[5] = PreemptivePriority(p6, NUM);
    result_array[6] = HRN(p7, NUM);
    result_array[7] = Custom(p8, NUM);
    result_array[8] = PreemptiveCustom(p9, NUM);
}

void eval(void){
    evaluation_result shortest_avg_waiting_time;
    evaluation_result shortest_avg_turnaround_time;

    shortest_avg_waiting_time.algorithm_idx = 0;
    shortest_avg_waiting_time.avg_waiting_time = result_array[0].avg_waiting_time;
    shortest_avg_turnaround_time.algorithm_idx = 0;
    shortest_avg_turnaround_time.avg_turnaround_time = result_array[0].avg_turnaround_time;

    for (int idx=1; idx<NUMofAlgorithms; idx++){
        if (shortest_avg_waiting_time.avg_waiting_time > result_array[idx].avg_waiting_time){
            shortest_avg_waiting_time.algorithm_idx = result_array[idx].algorithm_idx;
            shortest_avg_waiting_time.avg_waiting_time = result_array[idx].avg_waiting_time;
        }
        if (shortest_avg_turnaround_time.avg_turnaround_time > result_array[idx].avg_turnaround_time){
            shortest_avg_turnaround_time.algorithm_idx = result_array[idx].algorithm_idx;
            shortest_avg_turnaround_time.avg_turnaround_time = result_array[idx].avg_turnaround_time;
        }
    }

    printf("\n\n======================================================================\n");
    printf("==                                                                  ==\n");
    printf("==                        Evaluation  result                        ==\n");
    printf("==                                                                  ==\n");
    printf("======================================================================\n");
    printf("Total %d scheduling algorithms below\n", NUMofAlgorithms);
    for (int i=0; i<(NUMofAlgorithms/2)+1; i++) printf("%d. '%s'  ", i+1, algorithms[i]);
    printf("\n");
    for (int i=(NUMofAlgorithms/2)+1; i<NUMofAlgorithms; i++) printf("%d. '%s'  ", i+1, algorithms[i]);
    printf("\n\nThe shortest avg waiting time: [%s], %-2.2lf.\n", algorithms[shortest_avg_waiting_time.algorithm_idx], shortest_avg_waiting_time.avg_waiting_time);
    printf("The shortest avg turnaround time: [%s], %-2.2lf.\n\n", algorithms[shortest_avg_turnaround_time.algorithm_idx], shortest_avg_turnaround_time.avg_turnaround_time);

    for (int i=0; i<NUMofAlgorithms; i++){
        printf("----------------------------------------------------------------------\n");
        printf("Algorithm: [%s]\n", algorithms[result_array[i].algorithm_idx]);
        printf("\tAverage Waiting Time     : %-2.2lf\n", result_array[i].avg_waiting_time);
        printf("\tAverage Turnaround Time  : %-2.2lf\n", result_array[i].avg_turnaround_time);
        printf("\tTotal running Time       : %d\n", result_array[i].total_running_time);
        printf("\tIDLE_time                : %d\n", result_array[i].IDLE_time);
        printf("\tCPU utilization          : %-2.2lf%%\n\n", 100*((double)result_array[i].total_running_time-(double)result_array[i].IDLE_time)/(double)result_array[i].total_running_time);
    }
    printf("----------------------------------------------------------------------\n");
}

int main(void){
    result_array = (evaluation_result*) malloc(sizeof(evaluation_result)*NUMofAlgorithms);
    Schedule();

    eval();
    free(result_array);
}