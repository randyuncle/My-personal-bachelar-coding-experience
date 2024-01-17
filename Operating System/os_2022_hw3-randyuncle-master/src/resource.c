/**
 * This module is used to simulate a task that are using hardware resources.
 * The main goal is to block the  given id "resource_available" array to false
 * in order to representing that the hardware resource is used.
*/
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../include/resource.h"
#include "../include/task.h"
#include "../include/function.h"


bool resource_available[8] = {true, true, true, true, true, true, true, true};

void get_resources(int count, int *resources)
{
    while(1){
        int check = 0;
        for(int i = 0 ; i < count ; i++){
            int id = resources[i];
            if(resource_available[id])
                check++;
        }

        if(check < count){
            curr_task->state = WAITING;
            curr_task->wait.is_waiting_resource = 1;
            
            //insert the elements that needs to be checked
            curr_task->wait.resource_count = count;
            for(int i = 0 ; i < count ; i++)
                curr_task->wait.resources[i] = resources[i];

            push_wait();
            printf("Task %s is waiting resource.\n", curr_task->name);
            //switch to the scheduler
            swapcontext(&curr_task->context, &parent);
        }else
            break;
    }

    printf("Task %s gets resource ", curr_task->name);
    int resource_id = curr_task->resources_count;
    for(int i = 0 ; i < count ; i++){
        int id = resources[i];

        curr_task->resources[resource_id++] = id;
        resource_available[id] = false;
        printf("%d ", id);
    }
    printf("\n");
    curr_task->resources_count += count;
}

void release_resources(int count, int *resources)
{
    printf("Task %s release resource ", curr_task->name);
    for(int i = 0 ; i < count ; i++){
        int id = resources[i];
        resource_available[id] = true;
        printf("%d ", id);
    }
    printf("\n");
}
