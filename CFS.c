#include "CFS.h"

#define W_0 1024
#define TARGET_LATENCY 10 /* In seconds */

/************ RB Struct in Process Tree **************/

struct rb_info {
	int index;
	int a_runtime;
};

/******************* Helper Functions *****************/

static const int prio_to_weight[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};

void RB_insert_process(rb_red_blk_tree * tree, struct process ** processes, int index, int runtime) {

	struct rb_info * info = malloc(sizeof(struct rb_info));
	info -> index = index;
	info -> a_runtime = runtime;

	int W_i = (processes[index]->priority);
	int decay_factor = W_0 / W_i; 
	int * key = malloc(sizeof(int)); 
	*key = runtime * decay_factor; 

	RBTreeInsert(tree, key, info);
}

int calculate_timeslice(struct process ** processes, rb_red_blk_node min, int tree_total_weight) {

	struct rb_info curr_info = *(struct rb_info *) min.info;
	int curr_index = curr_info.index;
	int curr_prio = processes[curr_index]->priority;
	int curr_weight = prio_to_weight[curr_prio + 20];

	int timeslice = TARGET_LATENCY * ((float) curr_weight / (float) tree_total_weight);

	/* If timeslice is between 0 and 1, minimum timeslice should be 1 */
	if ((timeslice < 1) && (timeslice >= 0)) {
		timeslice = 1;
	}

	return timeslice;
}

/******************* Tree Functions *****************/

void Dest(void* a) {
	free((int*)a);
}

int Comp(const void* a,const void* b) {
	if( *(int*)a > *(int*)b) return(1);
	if( *(int*)a < *(int*)b) return(-1);
	return(0);
}

void Print(const void* a) {
	printf("%i",*(int*)a);
}

void InfoPrint(void* a) {
	struct rb_info * info = (struct rb_info *) a;
	printf("index: %d, a_runtime: %d", info->index, info->a_runtime);
}

void InfoDest(void *a){
	free((struct rb_info *)a);
}

rb_red_blk_node* getMin(rb_red_blk_tree* tree, rb_red_blk_node * x) {
	rb_red_blk_node* nil=tree->nil;
	rb_red_blk_node* root=tree->root;

	while(x->left != nil) {
		x=x->left;
	}
	return(x);
}

/**************** CFS Scheduler ****************/

void CFS_scheduler(struct process ** processes, int num_processes) {

	int tree_total_weight = 0;
	int time = 0;
	int processes_index = 0; /* processes that have been run / running */
	int processes_iter = 0; /* processes that have been added to queue */
	int timeslice = 0;

	// Creating pointer to root of RB tree 
	rb_red_blk_tree *tree = RBTreeCreate(Comp, Dest, InfoDest, Print, InfoPrint);

	// Creating local copy
	rb_red_blk_node min;
	struct rb_info info;
	int v_runtime;

	struct process * curr_process = NULL;
	int service_time = -1;
	int IS_PROCESS_RUNNING = 0;

	/* While there are still processes to be run */
	while (processes_index < num_processes) {

		/* push processes onto ready queue */
		while((processes_iter < num_processes) && (processes[processes_iter]->arrival_time <= time)) {

			// creating the node, inserting into RB tree
			int runtime = 0;
			RB_insert_process(tree, processes, processes_iter, runtime);
			
			// update total weight
			int prio = processes[processes_iter]->priority;
			tree_total_weight += prio_to_weight[prio + 20];		
			processes_iter++;
		}

		/* Update running node/process */
		if (tree->root->left != tree->nil) {
			
			if (IS_PROCESS_RUNNING) {
				// insert previously running node
				RB_insert_process(tree, processes, info.index, info.a_runtime);
			}

			// obtain min node from tree
			rb_red_blk_node * min_ptr = getMin(tree, tree->root->left);

			// set local copies
			min = *min_ptr;
			IS_PROCESS_RUNNING = 1;
			info = *(struct rb_info *) min_ptr->info;
			v_runtime = *(int *) min_ptr->key;
			curr_process = processes[info.index];
			service_time = curr_process->service_time;

			// compute new timeslice
			timeslice = calculate_timeslice(processes, min, tree_total_weight);

			// delete node from tree
			RBDelete(tree, min_ptr);
		}

		/* Currently running a process */
		if (IS_PROCESS_RUNNING) {

			/* Process is finished */
			if (info.a_runtime == service_time) {
				printf("<time %d> process %d is finished!\n", time, curr_process->pid);
				
				processes[processes_index]->exit_time = time;
				processes_index++;
				IS_PROCESS_RUNNING = 0;

				int prio = processes[info.index]->priority;
				tree_total_weight -= prio_to_weight[prio + 20];

				if (tree->root->left != tree->nil) {
					continue;
				}

			} else if (timeslice <= 0) {
				/* Timeslice is done, and process is not finished */
				RB_insert_process(tree, processes, info.index, info.a_runtime);
				continue;
				
			} else {
				/* continue running current timeslice on process */
				info.a_runtime++;
				timeslice--;
				printf("<time %d> process %d is running\n", time, curr_process->pid);
			}

		} else {
			/* No processes in ready queue */
			printf("<time %d> nothing is running\n", time);
		}

		wait(1);
		time++;
	}

	print_stats(processes, num_processes, time);
	RBTreeDestroy(tree);
}
