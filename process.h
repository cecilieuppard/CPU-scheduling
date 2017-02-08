#ifndef _PROCESS_H_
#define _PROCESS_H_

struct process {
	int pid;
	int arrival_time;
	int service_time;
	int priority;
	int exit_time;
};

#endif