#include "process.h"
#include "utils.h"
#include "queue.h"


evaluation_result FCFS(process *p, int len)
{
	//init
	int TotalTime = 0;
	int total_waiting_time = 0; // 총 대기 시간을 저장할 변수 선언 및 초기화
	int total_turnaround_time = 0; // 총 턴어라운드 타임을 저장할 변수 선언 및 초기화

	char gantt_record[20*len];
	char IO_record[20*len];


	process* job_queue = init_job_queue(p);
	process* ready_queue = init_ready_queue();
	process* wait_queue = init_wait_queue();
	process* terminated_queue = init_terminated_queue();

	evaluation_result result;
	result.IDLE_time = 0;

	int cpu_burst_time_record[len];
	int IO_burst_time_record[len];
	// fin. init

	for (int i=0; i<len; i++) {
		cpu_burst_time_record[i] = job_queue[i].CPU_burst_time;
		IO_burst_time_record[i] = 0;
	}
	
	
	

	printf("-------------------------------------------------------------------------------------\n");
	printf("\n\t[FCFS Scheduling Algorithm]\n\n");
	
	print_process_status(job_queue, len);
	job_queue = SORT_by_arrival(job_queue, len);
	while(1){
		if (DEBUG_MODE) printf("<<Time>> %d\n", TotalTime);
		//job scheduling
		for (int idx=0; idx<NoP_in_JQ; idx++){
			if (job_queue[0].arrival_time <= TotalTime){
				insert_ready_queue(ready_queue, pop_from_job_queue(job_queue, 0), 0);
			}
		}

		//I/O request 판단
		if (NoP_in_RQ > 0) {
			if (ready_queue[0].IO_request_time==0){
				if (DEBUG_MODE) printf("IO request. PID:%d, IO burst:%d\n", ready_queue[0].PID, ready_queue[0].IO_burst_time);
				// IO작업한 시간만큼 waiting time에서 빼주기 위해 arr에 추가
				IO_burst_time_record[ready_queue[0].PID] += ready_queue[0].IO_burst_time;
				insert_wait_queue(wait_queue, pop_from_ready_queue(ready_queue, 0), 0);
			}
		}
		//I/O processing 1: busrted process is poped from wait queue and inserted in ready queue
		//I/O processing 2: decreasing IO_burst_time for first waiting process
		if (NoP_in_WQ > 0){
			if (wait_queue[0].IO_burst_time == 0){
				// I/O 끝나서 ready queue로 복귀
				// printf("pop from wait queue\n");
				wait_queue[0].IO_occur_left--;
				if (wait_queue[0].IO_occur_left > 0){
					wait_queue[0].IO_burst_time = (rand()%10)+1;
					if (wait_queue[0].CPU_burst_time > 0) {
						// cpu burst내 범위에서 이 request_time만큼 process가 진행되면 IO_request를 받게 할 것임.
						wait_queue[0].IO_request_time = (rand()%wait_queue[0].CPU_burst_time);
						if (DEBUG_MODE) printf("%d's IO burst: %d, IO req: %d\n", wait_queue[0].PID, wait_queue[0].IO_burst_time, wait_queue[0].IO_request_time);
						// printf("%d's IO burst: %d, IO req: %d\n", wait_queue[0].PID, wait_queue[0].IO_burst_time, wait_queue[0].IO_request_time);
					} 
					else {
						wait_queue[0].IO_request_time = -999; // I/O 비활성화
					} 
				}
				else {
					wait_queue[0].IO_request_time = -999; 
				}
				insert_ready_queue(ready_queue, pop_from_wait_queue(wait_queue, 0), 0);

				if (NoP_in_WQ > 0){
					sprintf(&IO_record[TotalTime], "%d", wait_queue[0].PID); 
					// wait queue에 2개 이상이 있고, 처음p가 끝나서 나가면 그 다음p의 IO시간을 감소시켜야 함.
					wait_queue[0].IO_burst_time--;
					if (DEBUG_MODE) printf("%d(%d)->(%d)\n",wait_queue[0].PID, wait_queue[0].IO_burst_time+1, wait_queue[0].IO_burst_time);
				} 
				else IO_record[TotalTime] = 'X'; 
			}
			else{
				// printf("decreasing IO_burst_time PID: %d, IO busrt: %d\n", wait_queue[0].PID, wait_queue[0].IO_burst_time);
				sprintf(&IO_record[TotalTime], "%d", wait_queue[0].PID); 
				wait_queue[0].IO_burst_time--;
			}
			
		}
		else IO_record[TotalTime] = 'X';

		// 1. I/O burst 판단 2.I/O request 판단  3. ready queue에서 cpu burst판단
		//I/O request 판단
		if (NoP_in_RQ > 0) {
			for (int i=0; i<NoP_in_RQ; i++){
				if (ready_queue[i].IO_request_time==0){
					IO_burst_time_record[ready_queue[i].PID] += ready_queue[i].IO_burst_time;
					insert_wait_queue(wait_queue, pop_from_ready_queue(ready_queue, i), 0);
				}
			}
		}
		

		if (DEBUG_MODE) print_ready_queue(ready_queue);

		//CPU processing
		if (NoP_in_RQ > 0) {
			ready_queue[0].CPU_burst_time--; //FCFS
			ready_queue[0].IO_request_time--;
			// printf("PID:%d, remaining_burst: %d, waiting time: %d\n", ready_queue[0].PID, ready_queue[0].CPU_burst_time,ready_queue[0].waiting_time);

			//Gantt chart append
			sprintf(&gantt_record[TotalTime], "%d", ready_queue[0].PID); 

			//process terminated?
			if (ready_queue[0].CPU_burst_time <= 0){
				//calculate waiting&turnaround time
				// waiting time은 ready queue에서 대기한 시간만 count
				ready_queue[0].waiting_time = TotalTime+1 - cpu_burst_time_record[ready_queue[0].PID] - IO_burst_time_record[ready_queue[0].PID]- ready_queue[0].arrival_time;
				ready_queue[0].turnaround_time = TotalTime+1 - ready_queue[0].arrival_time;

				insert_terminated_queue(terminated_queue, pop_from_ready_queue(ready_queue, 0));
			}
		}
		else{
			//no one in ready queue
			// printf("idle\n");
			result.IDLE_time++;
			gantt_record[TotalTime] = 'X';
		}

		TotalTime++;
		if (NoP_in_TQ == NUM) break;
	}
	// printf("%d %d\n", cpu_burst_time_record[terminated_queue[3].PID],terminated_queue[3].arrival_time);
	// printf("wait time: ");
	for (int idx=0; idx<NUM; idx++){
		// printf("PID:%d, t:%d, reqT: %d\t", terminated_queue[idx].PID, terminated_queue[idx].waiting_time, terminated_queue[idx].IO_request_time);
		total_waiting_time = total_waiting_time + terminated_queue[idx].waiting_time;
		total_turnaround_time = total_turnaround_time + terminated_queue[idx].turnaround_time;
	}

	
	result.algorithm_idx = 0;
	result.avg_waiting_time = (double)total_waiting_time / (double)len;
	result.avg_turnaround_time = (double)total_turnaround_time / (double)len;
	result.total_running_time = TotalTime;

	
	print_cpu_gantt_chart(gantt_record, len, TotalTime);
	print_IO_gantt_chart(IO_record, len, TotalTime);

	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\n\tAverage Waiting Time     : %-2.2lf\n", result.avg_waiting_time);
	printf("\tAverage Turnaround Time  : %-2.2lf\n", result.avg_turnaround_time);

	return result;
}