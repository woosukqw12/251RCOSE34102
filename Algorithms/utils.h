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

// void print_process_status(process *job_queue, int len){
// 	for (int i=0; i<len; i++){
//         printf("[process %d]: (arrival time: %d/cpu burst: %d /IO burst %d /IO request %d / IO_occur_total %d/priority: %d)\n", \
//             job_queue[i].PID, job_queue[i].arrival_time, job_queue[i].CPU_burst_time, job_queue[i].IO_burst_time, job_queue[i].IO_request_time, job_queue[i].IO_occur_total, job_queue[i].priority);
//     }
// }

void print_process_status(process *job_queue, int len){
	for (int i=0; i<len; i++){
        printf("[process %d]: (arrival time: %d/cpu burst: %d /priority: %d/ IO_occur_total %d/ ", \
            job_queue[i].PID, job_queue[i].arrival_time, job_queue[i].CPU_burst_time, job_queue[i].priority, job_queue[i].IO_occur_total);
		for (int j=0; j<job_queue[i].IO_occur_total; j++){
			printf("%d번째 IO burst: %d, IO request: %d/ ", j, job_queue[i].IO_burst_time[j], job_queue[i].IO_request_time[j]);
		}
		printf("\n");
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