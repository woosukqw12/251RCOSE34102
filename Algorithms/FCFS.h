#include "process.h"
#include "utils.h"

// #include "SchedulerSimulator.c"
process* SORT_by_arrival(process *p, int len){
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

void print_gantt_chart(time_stamp *stamp, int len, int total_terminated_time){
	// stamp print
	for (int i=0;i<5;i++){
		printf("%d\t%d\t%d\n", stamp[i].PID, stamp[i].start, stamp[i].end);
	}

	int isIdle = 1;
	for (int i=0; i<=total_terminated_time; i++){
		for (int idx=0; idx<len; idx++){
			if ( stamp[idx].start <= i && i < stamp[idx].end ){
				printf("%d", stamp[idx].PID);
				isIdle = 0;
			}
		}
		if (isIdle == 1) printf("X");
		else isIdle = 1;
	}
	printf("\n");

	for (int blank=0; blank<stamp[0].start; blank++) printf(" ");
	for (int i=0; i<len; i++){
		printf("%d", stamp[i].start);
		for (int blank=0; blank<stamp[i].end-stamp[i].start-2; blank++){
			printf(" ");
		}
	}
	printf("%d", stamp[len-1].end);
}

// int max(int a, int b){
// 	if (a>b) return a;
// 	return b;
// }

void FCFS(process *p, int len)
{
	time_stamp* stamp = (time_stamp*) malloc(50*sizeof(time_stamp));
	p = SORT_by_arrival(p, len);
	int total_waiting_time = 0;
	// 총 대기 시간을 저장할 변수 선언 및 초기화
	int total_turnaround_time = 0;
	// 총 턴어라운드 타임을 저장할 변수 선언 및 초기화
	int total_terminated_time = p[0].arrival_time;
	// 총 반환 시간을 저장할 변수 선언 및 초기화

	printf("------------------------------------------------------------\n");
	for (int i=0; i<len; i++){
        printf("[process %d]: (arrival time: %d /cpu burst: %d /IO burst %d/priority: %d)\n", \
            p[i].PID, p[i].arrival_time, p[i].CPU_burst_time, p[i].IO_burst_time, p[i].priority);
    }

	/* 다음 프로세세스 부터 순차적으로 계산 */
	for (int i = 0; i < len; i++){
		/* 각 프로세스 계산 */
        p[i].terminated_time = total_terminated_time + p[i].CPU_burst_time;
        p[i].waiting_time = total_terminated_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].terminated_time - p[i].arrival_time;

		/* 실행된 프로세스만큼 증가 */
        total_terminated_time += p[i].CPU_burst_time;
        total_waiting_time += p[i].waiting_time;
		total_turnaround_time += p[i].turnaround_time;

		stamp[i].PID = p[i].PID;
		if (i==0) stamp[i].start = p[i].arrival_time;
		else stamp[i].start = max(p[i-1].terminated_time, p[i].arrival_time);
		stamp[i].end = p[i].terminated_time;
	}

	printf("\n\tFCFS Scheduling Algorithm\n\n");

	print_gantt_chart(stamp, len, total_terminated_time);

	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
	printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);

}