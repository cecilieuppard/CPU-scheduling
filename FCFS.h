#ifndef _FCFS_H_
#define _FCFS_H_

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include <unistd.h>
#include "utils.h"

#include <time.h>

void FCFS_scheduler(struct process ** processes, int num_processes);

#endif