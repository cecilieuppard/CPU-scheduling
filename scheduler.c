#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FCFS.h"
#include "CFS.h"
#include "process.h"

#define INIT_PROC_SIZE 1
#define LINE_SIZE 100
#define CFS "CFS"
#define FCFS "FCFS"

/************* Helper Functions *************/

void expand(struct process ***processes, int *process_array_capacity) {
	int old_size = *process_array_capacity;
	*process_array_capacity = old_size + 1;

	*processes = (struct process **) realloc(*processes, (*process_array_capacity) * sizeof(struct process *));
}

int cmp_arrival_time(const void *a, const void *b) {
	struct process **proc_A = (struct process **) a;
	struct process **proc_B = (struct process **) b;

	return (int) (*proc_A)->arrival_time - (*proc_B)->arrival_time;
}

void parse_input(FILE * fp, struct process ***processes, int *num_processes, int *process_array_capacity) {

	char line[LINE_SIZE];

    while (fgets(line, sizeof(line), fp)) {

    	/* Takes each line and sets info to process struct */
    	int param_counter = 1;
    	struct process *proc = malloc(sizeof(struct process));
		char *i = strtok(line, " ");  
		while (i != NULL) {
			if (!strcmp(i, " ")) {
    			continue;
    		}
    		if (param_counter == 1) {
    			proc->pid = atoi(i);
    		} else if (param_counter == 2) {
    			proc->arrival_time = atoi(i);
    		} else if (param_counter == 3) {
    			proc->service_time = atoi(i);
    		} else if (param_counter == 4) {    		
    			proc->priority = atoi(i);
    		}
    		param_counter++;
			i = strtok(NULL, " ");
		}
		if(*num_processes >= *process_array_capacity) {
			expand(processes, process_array_capacity);
		}

    	(*processes)[(*num_processes)] = proc;
    	(*num_processes)++;
    }
}

/************** Scheduler **************/

int main (int argc, char *argv[]) {

	if (argc != 3) {
		fprintf(stderr, "Error: Incorrect usage. Format: ./scheduler [input_file] [Algorithm]\n");
	} else {

		/* Reading in input file */
		FILE * fp = fopen(argv[1], "r");
		int process_array_capacity = INIT_PROC_SIZE;
		int num_processes = 0;
		struct process **processes = malloc(sizeof(struct process *) * process_array_capacity);

		/* Check file was successfully opened */ 
		if (fp != NULL) {
			parse_input(fp, &processes, &num_processes, &process_array_capacity);
			fclose(fp);
			qsort(processes, num_processes, sizeof(struct process *), cmp_arrival_time);
		} else {
			fprintf(stderr, "Error: Failure to open file\n");
			exit(EXIT_FAILURE);	
		}

		/* Setting Algorithm */
		char * algorithm = argv[2];
		if (!strncmp(algorithm, CFS, 3)) {
			CFS_scheduler(processes, num_processes);
		} else if (!strncmp(algorithm, FCFS, 4)) {
			FCFS_scheduler(processes, num_processes);			
		} else {
			/* Neither CFS nor FCFS */
			fprintf(stderr, "Error: Scheduling algorithm not supported.\n");
		}

		for(int i = 0; i < num_processes; i++) {
			free(processes[i]);
		}
		free(processes);
	}

	return 0;
}

