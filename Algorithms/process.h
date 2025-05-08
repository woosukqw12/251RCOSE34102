#pragma once


typedef struct{
    int PID;
    int arrival_time;
    int CPU_burst_time;
    int IO_burst_time;
    int IO_request_time;
    int priority;

    int terminated_time;
    int waiting_time;
    int turnaround_time;
} process;

typedef struct {
    int PID;
    int start;
    int end;
} time_stamp;


typedef struct {
    int algorithm_idx;
    double avg_waiting_time;
    double avg_turnaround_time;
} evaluation_result;
