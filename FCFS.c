#include "FCFS.h"

/****** Node Struct in Process Queue ********/

struct node {
	int index;
	int time_remaining;
	struct node * next;
};

/************* Helper Functions *************/

struct node * create_node(struct process ** processes, int index) {
	struct node * node_t = malloc(sizeof(struct node));

	node_t->index = index;
	node_t->time_remaining = processes[index]->service_time;
	node_t->next = NULL;

	return node_t;
}

void append_node(struct node ** head, struct node ** tail, struct process ** processes, int processes_iter) {
	struct node * next_process = create_node(processes, processes_iter);

	if ((*head) != NULL) {
		(*tail)->next = next_process;
		(*tail) = next_process;
	} else {
		(*head) = next_process;
		(*tail) = (*head);
	}
}

/************** FCFS Scheduler **************/

void FCFS_scheduler(struct process ** processes, int num_processes) {

	int time = 0;
	int processes_index = 0; /* processes that have been run */
	int processes_iter = 0; /* processes that have been added to queue */

	/* Creating head and tail pointer to ready queue */
	struct node * head = NULL;
	struct node * tail = head;

	while (processes_index < num_processes) {

		/* push processes onto ready queue */
		while((processes_iter < num_processes) && (processes[processes_iter]->arrival_time <= time)) {
			append_node(&head, &tail, processes, processes_iter);
			processes_iter++;
		}

		/* Processes in the ready queue */
		if (head != NULL) {
			struct process * curr_process = processes[processes_index];

			/* Done running */
			if (head->time_remaining <= 0) {
				printf("<time %d> process %d is finished!\n", time, curr_process->pid);
				struct node * to_delete = head;
				head = head->next;
				free(to_delete);

				processes[processes_index]->exit_time = time;
				processes_index++;

				/* If there is another process in the ready queue, continue to that process
				   Otherwise, run rest of loop */
				if (head != NULL) {
					continue;
				}

			} else {
				head->time_remaining--;
				printf("<time %d> process %d is running\n", time, curr_process->pid);
			}

		} else {
			printf("<time %d> nothing is running\n", time);
		}
		wait(1);
		time++;
	}
	print_stats(processes, num_processes, time);
}
