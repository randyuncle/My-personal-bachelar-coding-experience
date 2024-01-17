#include <stdlib.h>
#include <string.h>
#include "include/shell.h"
#include "include/command.h"
#include "include/task.h"

int main(int argc, char *argv[])
{
	signal(SIGTSTP, stop_handler); /*block the ctrl-z*/
	/*initializing task counter materials*/
	task_size = 0;
	ready_size = 0;
	wait_size = 0;
	task_list_f = NULL;
	task_list_r = NULL;
	ready_f = NULL;
	ready_r = NULL;
	wait_f = NULL;
	wait_r = NULL;

	/*simulator state*/
	reschedule = 1;
    is_idle = 0;

	/*save the scheduling algorithm type*/
	char *alg = argv[1];
	if(!strcmp(alg, "FCFS"))
		algorithm = FCFS;
	else if(!strcmp(alg, "PP"))
		algorithm = PP;
	else
		algorithm = RR;

	history_count = 0;
	for (int i = 0; i < MAX_RECORD_NUM; ++i)
    	history[i] = (char *)malloc(BUF_SIZE * sizeof(char));

	shell();

	for (int i = 0; i < MAX_RECORD_NUM; ++i)
    	free(history[i]);

	return 0;
}
