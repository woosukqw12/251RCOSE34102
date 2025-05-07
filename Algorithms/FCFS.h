#include "process.h"
#include "utils.h"
#include "queue.h"

void FCFS(process *p, int len)
{
	//init
	int TotalTime = 0;
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

	
	job_queue = SORT_by_arrival(job_queue, len);
	

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

			//Gantt chart append
			sprintf(&gantt_record[TotalTime], "%d", ready_queue[0].PID); 

			if (ready_queue[0].CPU_burst_time <= 0){
				ready_queue[0].waiting_time = TotalTime+1 - burst_time_record[ready_queue[0].PID] - ready_queue[0].arrival_time;
				ready_queue[0].turnaround_time = TotalTime+1 - ready_queue[0].arrival_time;

				insert_terminated_queue(terminated_queue, pop_from_ready_queue(ready_queue, 0));

			}
		}
		else{
			printf("idle\n");
			gantt_record[TotalTime] = 'X';
		}

		TotalTime++;
		if (NoP_in_TQ == NUM) break;
	}
	

	for (int idx=0; idx<NUM; idx++){
		total_waiting_time = total_waiting_time + terminated_queue[idx].waiting_time;
		total_turnaround_time = total_turnaround_time + terminated_queue[idx].turnaround_time;
	}

	printf("\n\tFCFS Scheduling Algorithm\n\n");
	print_gantt_chart(gantt_record, len, TotalTime);

	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
	printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);

	// time_stamp* stamp = (time_stamp*) malloc(50*sizeof(time_stamp));
	// p = SORT_by_arrival(p, len);
	// int total_waiting_time = 0;
	// // 총 대기 시간을 저장할 변수 선언 및 초기화
	// int total_turnaround_time = 0;
	// // 총 턴어라운드 타임을 저장할 변수 선언 및 초기화
	// int total_terminated_time = p[0].arrival_time;
	// // 총 반환 시간을 저장할 변수 선언 및 초기화

	// printf("------------------------------------------------------------\n");
	// for (int i=0; i<len; i++){
    //     printf("[process %d]: (arrival time: %d /cpu burst: %d /IO burst %d/priority: %d)\n", \
    //         p[i].PID, p[i].arrival_time, p[i].CPU_burst_time, p[i].IO_burst_time, p[i].priority);
    // }

	// /* 다음 프로세세스 부터 순차적으로 계산 */
	// for (int i = 0; i < len; i++){
	// 	/* 각 프로세스 계산 */
    //     p[i].terminated_time = total_terminated_time + p[i].CPU_burst_time;
    //     p[i].waiting_time = total_terminated_time - p[i].arrival_time;
    //     p[i].turnaround_time = p[i].terminated_time - p[i].arrival_time;

	// 	/* 실행된 프로세스만큼 증가 */
    //     total_terminated_time += p[i].CPU_burst_time;
    //     total_waiting_time += p[i].waiting_time;
	// 	total_turnaround_time += p[i].turnaround_time;

	// 	stamp[i].PID = p[i].PID;
	// 	if (i==0) stamp[i].start = p[i].arrival_time;
	// 	else stamp[i].start = max(p[i-1].terminated_time, p[i].arrival_time);
	// 	stamp[i].end = p[i].terminated_time;
	// }

	// printf("\n\tFCFS Scheduling Algorithm\n\n");

	// print_gantt_chart(stamp, len, total_terminated_time);

	// /* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	// printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
	// printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);

}