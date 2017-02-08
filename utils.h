#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include <unistd.h>

#include <time.h>

void wait(int seconds);
void print_stats(struct process ** processes, int num_processes, int time);

#endif