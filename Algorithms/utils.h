#pragma once

int max(int a, int b){
	if (a>b) return a;
	return b;
}



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

process* SORT_by_arrival_and_burst(process *p, int len){
    process temp;

	for (int iter=0; iter<len-1; iter++){
        for (int idx=0; idx<len-1; idx++){
            // 1순위: 도착 시간, 2순위: shortest job
            if ( p[idx].arrival_time > p[idx+1].arrival_time){
                temp = p[idx];
                p[idx] = p[idx+1];
                p[idx+1] = temp;
            }
        }
    }

	int time = 0;
	int idx = 0;
	int min_idx = 0;
	while (idx<len){
		// 현재 시간내에 도착한 process중에 shortest job을 찾음
		for (int i=idx+1; p[i].arrival_time <= time; i++){
			if (p[i].CPU_burst_time < p[min_idx].CPU_burst_time){
				min_idx = i;
			}
		}
		// 그걸 idx와 swap -> 결과적으로 SJF순으로 정렬됨
		temp = p[min_idx];
		p[min_idx] = p[idx];
		p[idx] = temp;
		// 시간, idx수정
		time = time + p[idx].CPU_burst_time;
		idx++;
		min_idx = idx;
	}

	return p;
}


process* SORT_by_arrival_and_priority(process *p, int len){
    process temp;

	for (int iter=0; iter<len-1; iter++){
        for (int idx=0; idx<len-1; idx++){
            // 1순위: 도착 시간, 2순위: lower value, higher priority
            if ( p[idx].arrival_time > p[idx+1].arrival_time){
                temp = p[idx];
                p[idx] = p[idx+1];
                p[idx+1] = temp;
            }
        }
    }

	int time = 0;
	int idx = 0;
	int min_idx = 0;
	while (idx<len){
		// 현재 시간내에 도착한 process중에 highest priority을 찾음(lower value, higher priority)
		for (int i=idx+1; p[i].arrival_time <= time; i++){
			if (p[i].priority < p[min_idx].priority){
				min_idx = i;
			}
		}
		// 그걸 idx와 swap -> 결과적으로 SJF순으로 정렬됨
		temp = p[min_idx];
		p[min_idx] = p[idx];
		p[idx] = temp;
		// 시간, idx수정
		time = time + p[idx].CPU_burst_time;
		idx++;
		min_idx = idx;
	}

	return p;
}

void print_process_status(process *job_queue, int len){
	for (int i=0; i<len; i++){
        printf("[process %d]: (arrival time: %d/cpu burst: %d /IO burst %d /IO request %d / IO_occur_left %d/priority: %d)\n", \
            job_queue[i].PID, job_queue[i].arrival_time, job_queue[i].CPU_burst_time, job_queue[i].IO_burst_time, job_queue[i].IO_request_time, job_queue[i].IO_occur_left, job_queue[i].priority);
    }
}

void print_cpu_gantt_chart(char* gantt_record, int len, int total_terminated_time){
	printf("\ncpu gantt chart:\t");
	for (int i=0; i<total_terminated_time; i++){
		printf("%c", gantt_record[i]);
	}
	printf("\n");
}

void print_IO_gantt_chart(char* gantt_record, int len, int total_terminated_time){
	printf("I/O gantt chart:\t");
	for (int i=0; i<total_terminated_time; i++){
		printf("%c", gantt_record[i]);
	}
	printf("\n\nDescription for chart: Each number represents the PID of the process that was running during that time unit,\n\
		\t\t\t\tand also X represents the IDLE in that time unit.\n");
}