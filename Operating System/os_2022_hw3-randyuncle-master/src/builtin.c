#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "../include/builtin.h"
#include "../include/command.h"
#include "../include/task.h"
#include "../include/resource.h"

int help(char **args)
{
	int i;
    printf("--------------------------------------------------\n");
  	printf("My Little Shell!!\n");
	printf("The following are built in:\n");
	for (i = 0; i < num_builtins(); i++) {
    	printf("%d: %s\n", i, builtin_str[i]);
  	}
	printf("%d: replay\n", i);
    printf("--------------------------------------------------\n");
	return 1;
}

int cd(char **args)
{
	if (args[1] == NULL) {
    	fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  	} else {
    	if (chdir(args[1]) != 0)
      		perror("lsh");
	}
	return 1;
}

int echo(char **args)
{
	bool newline = true;
	for (int i = 1; args[i]; ++i) {
		if (i == 1 && strcmp(args[i], "-n") == 0) {
			newline = false;
			continue;
		}
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
	}
	if (newline)
		printf("\n");

	return 1;
}

int exit_shell(char **args)
{
	return 0;
}

int record(char **args)
{
	if (history_count < MAX_RECORD_NUM) {
		for (int i = 0; i < history_count; ++i)
			printf("%2d: %s\n", i + 1, history[i]);
	} else {
		for (int i = history_count % MAX_RECORD_NUM; i < history_count % MAX_RECORD_NUM + MAX_RECORD_NUM; ++i)
			printf("%2d: %s\n", i - history_count % MAX_RECORD_NUM + 1, history[i % MAX_RECORD_NUM]);
	}
	return 1;
}

bool isnum(char *str)
{
	for (int i = 0; i < strlen(str); ++i) {
    	if(str[i] >= 48 && str[i] <= 57)
			continue;
        else
		    return false;
  	}
  	return true;
}

int mypid(char **args)
{
	char fname[BUF_SIZE];
	char buffer[BUF_SIZE];
	if(strcmp(args[1], "-i") == 0) {

	    pid_t pid = getpid();
	    printf("%d\n", pid);
	
	} else if (strcmp(args[1], "-p") == 0) {
	
		if (args[2] == NULL) {
      		printf("mypid -p: too few argument\n");
      		return 1;
    	}

    	sprintf(fname, "/proc/%s/stat", args[2]);
    	int fd = open(fname, O_RDONLY);
    	if(fd == -1) {
      		printf("mypid -p: process id not exist\n");
     		return 1;
    	}

    	read(fd, buffer, BUF_SIZE);
	    strtok(buffer, " ");
    	strtok(NULL, " ");
	    strtok(NULL, " ");
    	char *s_ppid = strtok(NULL, " ");
	    int ppid = strtol(s_ppid, NULL, 10);
    	printf("%d\n", ppid);
	    
		close(fd);

  	} else if (strcmp(args[1], "-c") == 0) {

		if (args[2] == NULL) {
      		printf("mypid -c: too few argument\n");
      		return 1;
    	}

    	DIR *dirp;
    	if ((dirp = opendir("/proc/")) == NULL){
      		printf("open directory error!\n");
      		return 1;
    	}

    	struct dirent *direntp;
    	while ((direntp = readdir(dirp)) != NULL) {
      		if (!isnum(direntp->d_name)) {
        		continue;
      		} else {
        		sprintf(fname, "/proc/%s/stat", direntp->d_name);
		        int fd = open(fname, O_RDONLY);
        		if (fd == -1) {
          			printf("mypid -p: process id not exist\n");
          			return 1;
        		}

        		read(fd, buffer, BUF_SIZE);
        		strtok(buffer, " ");
        		strtok(NULL, " ");
        		strtok(NULL, " ");
		        char *s_ppid = strtok(NULL, " ");
		        if(strcmp(s_ppid, args[2]) == 0)
		            printf("%s\n", direntp->d_name);

        		close(fd);
     		}
	   	}
    	
		closedir(dirp);
	
	} else {
    	printf("wrong type! Please type again!\n");
  	}
	
	return 1;
}

int add(char **args)
{
	char *name = args[1];
	char *function = args[2];
	int priority = atoi(args[3]);

	curr_task = createNode(name, function, priority);
	if(!task_list_f || !task_list_r){
        task_list_f = curr_task;
        task_list_r = task_list_f;
    }else{
        task_list_f->next = curr_task;
        task_list_f = task_list_f->next;
    }

	push_ready();

	getcontext(&curr_task->context);
    curr_task->stack = (char *)malloc(sizeof(char) * 16384);
    curr_task->context.uc_stack.ss_sp = curr_task->stack;
    curr_task->context.uc_stack.ss_size = 16384;

    for(int i = 0 ; i < 14 ; i++){
        if(!strcmp(curr_task->function, task_str[i])){
            makecontext(&curr_task->context, task_func[i], 0);
            break;
        }
    }

	printf("Task %s is ready.\n", name);

	return 1;
}

int del(char **args)
{
	char *name = args[1];

	task_p task, prev = NULL;
    for(task = task_list_r ; task != NULL ; task = task->next){
        if(!strcmp(task->name, name)){
			if(task == task_list_r){
				//tail case
				task_list_r = task_list_r->next;
				if(task == task_list_f)
					task_list_f = NULL; //task_size = 1
			}
			else if(task == task_list_f){
				//head case
				task_list_f = prev;
				task_list_f->next = NULL;
			}
			else{
				//normal case
				prev->next = task->next;
			}
            //break;
        }else
        	prev = task;
    }

	if(ready_size > 0){
		q prev_r = NULL;
		for(q find_r = ready_r ; find_r != NULL ; find_r = find_r->next){
			if(!strcmp(find_r->task->name, name)){
				ready_size--;
				if(find_r == ready_r){
					//tail case
					ready_r = ready_r->next;
					if(find_r == ready_f){
						//task_size = 1
						ready_f = NULL; 
						ready_r = NULL;
					}	
				}
				else if(find_r == ready_f){
					//head case
					ready_f = prev_r;
					ready_f->next = NULL;
				}
				else{
					//normal case
					prev_r->next = find_r->next;
				}
				break;
			}
			prev_r = find_r;
		}
	}

	if(wait_size > 0){
		q prev_w = NULL;
		for(q find_w = wait_r ; find_w != NULL ; find_w = find_w->next){
			if(!strcmp(find_w->task->name, name)){
				if(task->resources_count > 0){
					for(int i = 0 ; i < task->resources_count ; i++){
						int id = task->resources[i];
						resource_available[id] = true;
					}
				}
				wait_size--;
				if(find_w == wait_r){
					//tail case
					wait_r = wait_r->next;
					if(find_w == wait_f){
						//task_size = 1
						wait_f = NULL;
						wait_r = NULL;
					}
				}
				else if(find_w == wait_f){
					//head case
					wait_f = prev_w;
					wait_f->next = NULL;
				}
				else{
					//normal case
					prev_w->next = find_w->next;
				}
				break;
			}
			prev_w = find_w;
		}
	}

    free(task);

    printf("Task %s is killed.\n", name);
	return 1;
}

int ps(char **args)
{
	printf(" TID|       name|      state| running| waiting| turnaround| resources| priority\n");
	printf("-------------------------------------------------------------------------------\n");
	for(task_p t = task_list_r ; t != NULL ; t = t->next){
		if(t->state == TERMINATE){
			if(t->resources_count > 0){
				char buffer[11];
				if(t->resources_count == 2)
					sprintf(buffer, "%d %d", t->resources[0], t->resources[1]);
				else
					sprintf(buffer, "%d %d %d", t->resources[0], t->resources[1], t->resources[2]);
					
				printf("%4d|%11s|%11s|%8d|%8d|%11d|%10s|%9d\n", t->TID, t->name, "TERMINATE", t->running, t->waiting, t->turnaround, buffer, t->priority);
			}else
				printf("%4d|%11s|%11s|%8d|%8d|%11d|%10s|%9d\n", t->TID, t->name, "TERMINATE", t->running, t->waiting, t->turnaround, "none", t->priority);
		}
		else if(t->state == READY){
			if(t->resources_count > 0){
				char buffer[11];
				if(t->resources_count == 2)
					sprintf(buffer, "%d %d", t->resources[0], t->resources[1]);
				else
					sprintf(buffer, "%d %d %d", t->resources[0], t->resources[1], t->resources[2]);
				printf("%4d|%11s|%11s|%8d|%8d|%11s|%10s|%9d\n", t->TID, t->name, "READY", t->running, t->waiting, "none", buffer, t->priority);
			}else
				printf("%4d|%11s|%11s|%8d|%8d|%11s|%10s|%9d\n", t->TID, t->name, "READY", t->running, t->waiting, "none", "none", t->priority);
		}else if(t->state == WAITING){
			if(t->resources_count > 0){
				char buffer[11];
				if(t->resources_count == 2)
					sprintf(buffer, "%d %d", t->resources[0], t->resources[1]);
				else
					sprintf(buffer, "%d %d %d", t->resources[0], t->resources[1], t->resources[2]);
				printf("%4d|%11s|%11s|%8d|%8d|%11s|%10s|%9d\n", t->TID, t->name, "WAITING", t->running, t->waiting, "none", buffer, t->priority);
			}else
				printf("%4d|%11s|%11s|%8d|%8d|%11s|%10s|%9d\n", t->TID, t->name, "WAITING", t->running, t->waiting, "none", "none", t->priority);
		}else{
			if(t->resources_count > 0){
				char buffer[11];
				if(t->resources_count == 2)
					sprintf(buffer, "%d %d", t->resources[0], t->resources[1]);
				else
					sprintf(buffer, "%d %d %d", t->resources[0], t->resources[1], t->resources[2]);
				printf("%4d|%11s|%11s|%8d|%8d|%11s|%10s|%9d\n", t->TID, t->name, "RUNNING", t->running, t->waiting, "none", buffer, t->priority);
			}else
				printf("%4d|%11s|%11s|%8d|%8d|%11s|%10s|%9d\n", t->TID, t->name, "RUNNING", t->running, t->waiting, "none", "none", t->priority);
		}
	}
	printf("\n");
	return 1;
}

int start(char **args)
{
	/*reset the resume signal*/
	need_resume = 0;

	/*switch to simulator*/
	printf("start simulating\n");
	simulator();

	if(!need_resume)
		/*switch back to shell mode without ctrl-z*/
		printf("simulation over\n");
	
	return 1;
}

const char *builtin_str[] = {
 	"help",
 	"cd",
	"echo",
 	"exit",
 	"record",
	"mypid",
	"add",
	"del",
	"ps",
	"start"
};

const int (*builtin_func[]) (char **) = {
	&help,
	&cd,
	&echo,
	&exit_shell,
  	&record,
	&mypid,
	&add,
	&del,
	&ps,
	&start
};

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}
