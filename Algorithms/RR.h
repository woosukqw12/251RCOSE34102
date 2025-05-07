#include "process.h"
#include "utils.h"
#include "queue.h"

#define TimeQuantum 4

// #include "SchedulerSimulator.c"
process* SORT_by_arrival_RR(process *p, int len){
    process temp;

    //bubble sort
    for (int iter=0; iter<len-1; iter++){
        for (int idx=0; idx<len-1; idx++){
            if (p[idx].arrival_time > p[idx+1].arrival_time){
                temp = p[idx];
                p[idx] = p[idx+1];
                p[idx+1] = temp;
            }
        }
    }
	return p;
}

void print_gantt_chart_RR(char* gantt_record, int len, int total_terminated_time){
	for (int i=0; i<total_terminated_time; i++){
		printf("%c", gantt_record[i]);
	}
	printf("\n");
}


void RR(process *p, int len)
{
	//init
	int TotalTime = 0;
	int runningTime = 0;
	int total_waiting_time = 0; // 총 대기 시간을 저장할 변수 선언 및 초기화
	int total_turnaround_time = 0; // 총 턴어라운드 타임을 저장할 변수 선언 및 초기화

	char gantt_record[20*len];

	process* job_queue = init_job_queue(p);
	process* ready_queue = init_ready_queue();
	process* wait_queue = init_wait_queue();
	process* terminated_queue = init_terminated_queue();

	int burst_time_record[len];
	// fin. init

	for (int i=0; i<len; i++) burst_time_record[i] = job_queue[i].CPU_burst_time;

	
	job_queue = SORT_by_arrival_RR(job_queue, len);
	

	printf("------------------------------------------------------------\n");
	for (int i=0; i<len; i++){
        printf("[process %d]: (arrival time: %d /cpu burst: %d /IO burst %d/priority: %d)\n", \
            job_queue[i].PID, job_queue[i].arrival_time, job_queue[i].CPU_burst_time, job_queue[i].IO_burst_time, job_queue[i].priority);
    }

	while(1){
		//job scheduling
		for (int idx=0; idx<NoP_in_JQ; idx++){
			if (job_queue[0].arrival_time <= TotalTime){
				insert_ready_queue(ready_queue, pop_from_job_queue(job_queue, 0), 0);
			}
		}

		//CPU processing
		if (NoP_in_RQ > 0) {
			ready_queue[0].CPU_burst_time--; //FCFS
			// printf("PID:%d, remaining_burst: %d, waiting time: %d\n", ready_queue[0].PID, ready_queue[0].CPU_burst_time,ready_queue[0].waiting_time);
			
			runningTime++;

			//Gantt chart append
			sprintf(&gantt_record[TotalTime], "%d", ready_queue[0].PID); 

			if (ready_queue[0].CPU_burst_time <= 0){
				ready_queue[0].waiting_time = TotalTime+1 - burst_time_record[ready_queue[0].PID] - ready_queue[0].arrival_time;
				ready_queue[0].turnaround_time = TotalTime+1 - ready_queue[0].arrival_time;

				insert_terminated_queue(terminated_queue, pop_from_ready_queue(ready_queue, 0));
				//running state바뀌니까 시간count 초기화
				runningTime = 0;
			}
		}
		else{
			printf("idle\n");
			gantt_record[TotalTime] = 'X';
			runningTime = 0;
		}
		

		// TimeQuantum을 다 소비했는가? -> 했다면, ready queue의 맨 뒤로 보내기
		if (runningTime == TimeQuantum){
			insert_ready_queue(ready_queue, pop_from_ready_queue(ready_queue, 0), 0);

			runningTime = 0;
		}
		TotalTime++;
		// printf(">>Total Time: %d\trunning Time: %d\n", TotalTime, runningTime);
		if (NoP_in_TQ == NUM) break;
	}
	

	for (int idx=0; idx<NUM; idx++){
		total_waiting_time = total_waiting_time + terminated_queue[idx].waiting_time;
		total_turnaround_time = total_turnaround_time + terminated_queue[idx].turnaround_time;
	}

	printf("\n\tRR Scheduling Algorithm\n\n");
	print_gantt_chart_RR(gantt_record, len, TotalTime);

	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
	printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);

}