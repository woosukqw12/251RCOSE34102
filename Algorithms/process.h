#pragma once


typedef struct{
    int PID;
    int arrival_time;
    int CPU_burst_time;
    int *IO_burst_time;
    int *IO_request_time;
    int IO_occur_total;
    int IO_occur_cur;
    int priority;

    int terminated_time;
    int waiting_time;
    int turnaround_time;
    int response_time;

    int ticket;
} process;

typedef struct {
    int algorithm_idx;
    double avg_waiting_time;
    double avg_turnaround_time;
    int total_running_time;
    int IDLE_time;
} evaluation_result;
