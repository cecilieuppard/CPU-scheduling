#include "utils.h"

void wait(int seconds) {
	clock_t endwait;
	endwait = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < endwait);
}

void print_stats(struct process ** processes, int num_processes, int time) {
	float total_service_time = 0;
	float total_TAT = 0;
	float norm_TAT = 0;

	for (int i = 0; i < num_processes; i++) {
		float service_time = processes[i]->service_time;
		float TAT = (processes[i]->exit_time) - (processes[i]->arrival_time);

		total_service_time += service_time;
		total_TAT += TAT;
		norm_TAT += TAT / service_time;

	}

	printf("\n-------------------------\n");
	printf("CPU Usage: %.2f%\n", (total_service_time / (float) time) * 100.00);
	printf("Average TAT: %.2f \n", total_TAT / num_processes);
	printf("Average normalized TAT: %.2f \n", norm_TAT / num_processes);
	printf("---------------------------\n");
}
