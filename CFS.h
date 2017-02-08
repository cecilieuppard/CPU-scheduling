#ifndef _CFS_H_
#define _CFS_H_

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include <unistd.h>
#include "utils.h"
#include "red_black_tree.h"
#include <math.h>
#include <time.h>

void CFS_scheduler(struct process ** processes, int num_processes);

#endif