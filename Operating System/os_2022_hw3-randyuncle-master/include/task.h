#ifndef TASK_H
#define TASK_H
#endif

#ifndef alg_size
#define alg_size 10

/*task states*/
#define WAITING 0
#define READY 1
#define RUNNING 2
#define TERMINATE 3

/*algorithm state*/
#define FCFS 0
#define PP 1
#define RR 2

#include <ucontext.h>
#include <sys/time.h>
#include <signal.h>

typedef struct waiting_properties{
    /*signals*/
    int is_waiting_resource;
    int issleep;

    /*sleep properties*/
    int sleep_counter;
    int sleep_ms;

    /*resource properties*/
    int resources[8];
    int resource_count;
}waiting_p;

/**
 * The way to make a user thread -> ucontext !
 * Also, by the course, if we want to maintain a huge amount of threads,
 * we make a "thread pool" to hold the input threads.
 * Therefore, the following struct appears.
*/
typedef struct task_pool *task_p;
typedef struct task_pool{
    ucontext_t context;
    char *stack;

    int TID;
    char *name;
    char function[15];
    int state; /*0 -> waiting, 1 -> ready, 2 -> running, 3 -> terminate*/

    /*timing*/
    int running;
    int waiting;
    int turnaround;

    /*resources*/
    int resources[8];
    int resources_count; /*counter for the amount of resoureces*/

    int priority; /*only for priority scheduling*/
    waiting_p wait; /*the elements for waiting*/
    task_p next; /*link_list pointer*/
};
task_p task_list_f, task_list_r;/*the input linked list*/
task_p curr_task; char *run_task_name;/*current running task*/
int reschedule, is_idle, is_end;

/**
 * this is a linked list for the ready queue
*/
typedef struct queue *q;
struct queue {
    //ucontext_t context;
    task_p task;
    q next;
};
q ready_r, ready_f; 
q wait_r, wait_f;
int ready_size, wait_size;

/*tick for the task*/
struct itimerval task_tick;

//the context properties for the functions
ucontext_t parent; /*simulator*/

int task_size;/*the total size of the task*/

int rr_sign_count;
int need_resume;/*if this "start" command needs to do the resume*/

//algorithn type
int algorithm;

/*timer handler*/
void startTimer();
void endTimer();

/*scheduling algorithm*/
void scheduling();

/*signal handler*/
void push_ready_signal(q);
void signal_handler(int);
void stop_handler(int);

/*simulating*/
void dispatch();
void simulator();

/*tasks build*/
q createNode_r();
task_p pop_ready();
void push_ready();
q createNode_w();
void push_wait();
task_p createNode(char*, char* , int);

/*task APIs*/
void task_sleep(int);
void task_exit();

extern const char *task_str[];

extern const int (*task_func[]) (char **);

#endif
