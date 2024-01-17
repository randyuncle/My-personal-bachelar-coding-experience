#include <ucontext.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include "../include/task.h"
#include "../include/function.h"
#include "../include/resource.h"

/*****************timer handler*****************/

void startTimer(){
    /*start the timer*/
    is_end = 0;

    signal(SIGVTALRM, signal_handler);
    task_tick.it_value.tv_sec = 0;
    task_tick.it_value.tv_usec = 1;
    task_tick.it_interval.tv_sec  = 0;
    task_tick.it_interval.tv_usec = 10000; //時間間隔 10(ms)

    if (setitimer(ITIMER_VIRTUAL, &task_tick, NULL)){
        printf("setitimer error\n");
        exit(1);
    }
    return;
}

void endTimer(){
    /*timer stop*/
    is_end = 1;

    task_tick.it_value.tv_sec = 0;
    task_tick.it_value.tv_usec = 0;
    task_tick.it_interval.tv_sec  = 0;
    task_tick.it_interval.tv_usec = 0; //時間暫停

    if (setitimer(ITIMER_VIRTUAL, &task_tick, NULL)){
        printf("setitimer error\n");
        exit(1);
    }
    return;
}

/*****************task data structure construct*****************/

task_p createNode(char *name, char *function, int priority){
    task_p node = (task_p)malloc(sizeof(struct task_pool));

    node->TID = ++task_size;

    node->name = (char*)malloc(sizeof(name));
    strcpy(node->name, name);
    strcpy(node->function, function);
    
    node->state = READY;
    node->running = 0;
    node->waiting = 0;
    node->turnaround = 0;
    node->resources_count = 0;

    node->priority = priority;

    node->wait.is_waiting_resource = 0;
    node->wait.issleep = 0;

    node->next = NULL;

    return node;
}

/*****************ready queue data structure construct*****************/

q createNode_r(){
    q node = (q)malloc(sizeof(q));

    node->task = curr_task;
    node->next = NULL;

    return node;
}

task_p pop_ready(){
    task_p node = ready_r->task;

    ready_r = ready_r->next;
    ready_size--;
    //printf("ready_size = %d\n", ready_size);

    return node;
}

void push_ready(){
    if(ready_size == 0){
        ready_f = createNode_r();
        ready_r = ready_f;
    }else{
        if(algorithm == PP){
            q node = createNode_r(), prev = NULL;
            for(q curr = ready_r ; curr != NULL ; curr = curr->next){
                if(node->task->priority < curr->task->priority){
                    if(prev){
                        prev->next = node;
                        node->next = curr;
                    }else{
                        //head case
                        node->next = curr;
                        ready_r = node;
                    }
                    break;
                }
                prev = curr;
            }

            if(prev && prev == ready_f){
                //tail case
                ready_f->next = node;
                ready_f = ready_f->next;
            }
        }else{
            ready_f->next = createNode_r();
            ready_f = ready_f->next;
        }
    }

    ready_size++;
    //printf("ready_size = %d\n", ready_size);
    return;
}

/*****************waiting queue data structure construct*****************/

q createNode_w(){
    q node = (q)malloc(sizeof(q));

    node->task = curr_task;
    node->next = NULL;

    return node;
}

void push_wait(){
    if(wait_size == 0){
        wait_f = createNode_w();
        wait_r = wait_f;
    }else{
        wait_f->next = createNode_w();
        wait_f = wait_f->next;
    }

    wait_size++;
    return;
}

/*****************scheduling algorithms*****************/

void scheduling(){
    if(curr_task->state != RUNNING){
        /*not running -> change the state*/
        rr_sign_count = 0;
        reschedule = 1;
    }
    else{
        /*the special cases*/
        switch(algorithm){
            case FCFS:
                break;
            case RR:
                rr_sign_count++;
                if(rr_sign_count >= 3 && ready_size > 1){
                    rr_sign_count = 0;
                    curr_task->state = READY;
                    push_ready();
                    reschedule = 1;
                    printf("Task %s is ready.\n", curr_task->name);
                    swapcontext(&curr_task->context, &parent);
                }
                break;
            case PP:
                if(ready_r->task->priority < curr_task->priority){
                    curr_task->state = READY;
                    push_ready();
                    reschedule = 1;
                    printf("Task %s is ready.\n", curr_task->name);
                    swapcontext(&curr_task->context, &parent);
                }
                break;
        }
    }
    return;
}

void push_ready_signal(q curr){
    task_p task = curr->task;
    if(ready_size == 0){
        //ready queue from empty to having elements
        ready_f = curr;
        ready_r = ready_f;
    }else if(algorithm == RR || algorithm == FCFS){
        //RR case and FCFS case
        if(ready_size > 0){
            ready_f->next = curr;
            ready_f = ready_f->next;
        }else{
            ready_f = curr;
            ready_r = ready_f;
        }
    }else{
        //PP case
        q prev = NULL;
        for(q ins_ready = ready_r ; ins_ready != NULL ; ins_ready = ins_ready->next){
            if(task->priority < ins_ready->task->priority){
                if(prev){
                    //normal case
                    prev->next = curr;
                    curr->next = ins_ready;
                }else{
                    //head case
                    curr->next = ins_ready;
                    ready_r = curr;
                }
                break;
            }
            prev = ins_ready;
        }

        if(prev && prev == ready_f){
            //tail case
            prev->next = curr;
            curr->next = NULL;
        }
    }

    ready_size++;
    return;
}

void signal_handler(int signal){
    if(!is_end){
        /*Calculate all task-related time (granularity: 10ms)*/
        for(q curr = ready_r ; curr != NULL && ready_size > 0 ; curr = curr->next)
            if(curr->task->state == READY)
                curr->task->waiting++;

        q prev_w = NULL;
        for(q curr = wait_r ; curr != NULL && wait_size > 0 ; curr = curr->next){
            task_p task = curr->task;
            if(task->state == WAITING){
                int insert_ready = 0;
                task->waiting++;

                /*sleeping check*/
                if(task->wait.issleep){
                    task->wait.sleep_counter++;
                    if(task->wait.sleep_ms == task->wait.sleep_counter){
                        insert_ready = 1;
                        task->wait.issleep = 0;
                    }
                }

                /*resource checking*/
                if(task->wait.is_waiting_resource){
                    int check = 0;
                    for(int i = 0 ; i < task->wait.resource_count ; i++){
                        int id = task->wait.resources[i];
                        if(resource_available[id])
                            check++;
                    }

                    if(check == task->wait.resource_count){
                        insert_ready = 1;
                        task->wait.is_waiting_resource = 0;
                    }
                }

                //add into ready queue
                if(insert_ready){
                    printf("Task %s is ready\n", curr->task->name);
                    task->state = READY;

                    if(curr == wait_r){
                        //tail case
                        wait_r = wait_r->next;
                        if(curr == wait_f)
                            wait_f = NULL; //task_size = 1
                    }
                    else if(curr == wait_f){
                        //head case
                        wait_f = prev_w;
                        wait_f->next = NULL;
                    }
                    else{
                        //normal case
                        prev_w->next = curr->next;
                    }

                    wait_size--;
                    push_ready_signal(curr);
                }else{
                    prev_w = curr;
                }
            }
        }

        if(curr_task->state == RUNNING)
            curr_task->running++;

        /*scheduling trigger*/
        if(!reschedule){
            if(ready_size > 0){
                //printf("%d\n", ready_size);
                is_idle = 0;

                /*scheduling algorithm*/
                scheduling();
            }else if(wait_size > 0){
                is_idle++;
                if(is_idle > 2)
                    is_idle = 2;
            }
        }
    }
}

void stop_handler(int signal){ 
    need_resume = 1;
    is_end = 1;
    if(curr_task->state == RUNNING){
        curr_task->state = READY;
        q node = (q)malloc(sizeof(struct queue));
        node->task = curr_task;
        node->next = ready_r;
        ready_r = node;
        ready_size++;
        printf("Task %s is ready.\n", curr_task->name);
        swapcontext(&curr_task->context, &parent);
    }
}

void dispatch(){
    curr_task = pop_ready();
    curr_task->state = RUNNING;
    printf("Task %s is running.\n", curr_task->name);
    reschedule = 0; /*wait for dispatcher get the elements*/
    swapcontext(&parent, &curr_task->context);
}

/**
 * the function bridges simulator and the main
*/
void simulator(){
    /*start the timer*/
    startTimer();

    /*then wait for the signal to trigger if it's time to do the scheduling*/
    do{
        if(is_end)
            break;
        else if(reschedule){
            dispatch();
        }else if(is_idle == 1){
            is_idle++;
            printf("CPU idle\n");
        }

        for(int i = 0 ; i < 250 ; i++); //little delay for having the data be saved properly
    }
    while(ready_size > 0 || wait_size > 0);

    /*timer stop*/
    endTimer();
}


/*****************task APIs*****************/

void task_sleep(int ms_10)
{
    curr_task->state = WAITING;
    push_wait();
    curr_task->wait.issleep = 1;
    curr_task->wait.sleep_counter = 0;
    curr_task->wait.sleep_ms = ms_10;

    printf("Task %s is sleeping.\n", curr_task->name);
    swapcontext(&curr_task->context, &parent);
}

void task_exit()
{
    curr_task->turnaround = curr_task->running + curr_task->waiting;
    curr_task->state = TERMINATE;

    printf("Task %s is terminated.\n", curr_task->name);
    swapcontext(&curr_task->context, &parent);
}

const char *task_str[] = {
 	"test_exit",
 	"test_sleep",
	"test_resource1",
 	"test_resource2",
 	"idle",
	"task1",
	"task2",
	"task3",
	"task4",
	"task5",
    "task6",
    "task7",
    "task8",
    "task9"
};

const int (*task_func[]) (char **) = {
	&test_exit,
	&test_sleep,
	&test_resource1,
	&test_resource2,
  	&idle,
	&task1,
	&task2,
	&task3,
	&task4,
	&task5,
    &task6,
    &task7,
    &task8,
    &task9
};