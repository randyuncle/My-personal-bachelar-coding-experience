/**
 * The code refers to the "Linux Kernal Module Programming Guide - Section 7.2"
*/
#include <linux/kernel.h> /* We're doing kernel work */ 
#include <linux/module.h> /* Specifically, a module */ 
#include <linux/proc_fs.h> /* Necessary because we use the proc fs */ 
#include <linux/uaccess.h> /* for copy_from_user */ 
#include <linux/version.h> 
#include <linux/pid.h>
#include <linux/pid_namespace.h>
 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0) 
#define HAVE_PROC_OPS 
#endif 
 
#define PROCFS_MAX_SIZE 1024 
#define PROCFS_NAME "thread_info" 

long thread_id[1024]; /*saving the sending thread ids*/
int id_counter = 0; /*counter for the thread_id*/

/*this data type is used to save the information of each thread ids*/
typedef struct savings{
    long int thread_id;
    long int utime;
    long int context_switch;
}savings;
savings threads_array[1024];
int arrCounter = 0; /*counter for the threads_array*/

struct pid *pid_struct; /*the pid strucure of the given pid*/
struct task_struct *task; /*the task strucure of the given pid structure*/

/* This structure hold information about the /proc file */ 
static struct proc_dir_entry *our_proc_file; 
 
/* The buffer used to store character for this module */ 
static char procfs_buffer[PROCFS_MAX_SIZE]; 
 
/* The size of the buffer */ 
static unsigned long procfs_buffer_size = 0; 
 
 
/* This function is called then the /proc file is read */ 
static ssize_t procfile_read(struct file *filePointer, char __user *buffer, size_t buffer_length, loff_t *offset) 
{ 
    char s[128], buf[1024];
    int i = 0;
    /*sending string making*/
    while(i < arrCounter){
        snprintf(s, 128,"%ld %ld %ld ", (long int)threads_array[i].thread_id, (long int)threads_array[i].utime, (long int)threads_array[i].context_switch);
        if(!i)
            strcpy(buf, s);
        else
            strcat(buf, s);
        i++;
    }
    procfs_buffer_size = sizeof(buf); 
    ssize_t ret = procfs_buffer_size; 
 
    if (copy_to_user(buffer, buf, procfs_buffer_size)) { 
        pr_info("copy_to_user failed\n"); 
        ret = 0; 
    } else { 
        //pr_info("procfile read %s\n", filePointer->f_path.dentry->d_name.name); 
        *offset += procfs_buffer_size; 
    } 
 
    return ret; 
} 
 
/* This function is called with the /proc file is written. */ 
static ssize_t procfile_write(struct file *file, const char __user *buff, size_t len, loff_t *off) 
{ 
    procfs_buffer_size = len; 
    if (procfs_buffer_size > PROCFS_MAX_SIZE) 
        procfs_buffer_size = PROCFS_MAX_SIZE; 
 
    if (copy_from_user(procfs_buffer, buff, procfs_buffer_size)) 
        return -EFAULT; 
    
    /*get thread id*/
    thread_id[id_counter] = simple_strtoul(procfs_buffer, NULL, 0); 
    pid_struct = find_get_pid(thread_id[id_counter]);

    /*get the current task_struct of the given thread id*/
    task = pid_task(pid_struct, PIDTYPE_PID);

    /*update the thread structure array*/
    threads_array[arrCounter].thread_id = (long int)thread_id[id_counter++];
    threads_array[arrCounter].utime = (long int)task->utime / 1000000;
    threads_array[arrCounter++].context_switch = (long int)task->nvcsw + task->nivcsw;

    procfs_buffer[procfs_buffer_size & (PROCFS_MAX_SIZE - 1)] = '\0'; 
    *off += procfs_buffer_size; 
 
    return procfs_buffer_size; 
} 
 
#ifdef HAVE_PROC_OPS 
static const struct proc_ops proc_file_fops = { 
    .proc_read = procfile_read, 
    .proc_write = procfile_write, 
}; 
#else 
static const struct file_operations proc_file_fops = { 
    .read = procfile_read, 
    .write = procfile_write, 
}; 
#endif 
 
static int __init My_proc_init(void) 
{ 
    our_proc_file = proc_create(PROCFS_NAME, 0777, NULL, &proc_file_fops); 
    if (NULL == our_proc_file) { 
        proc_remove(our_proc_file); 
        pr_alert("Error:Could not initialize /proc/%s\n", PROCFS_NAME); 
        return -ENOMEM; 
    } 
 
    pr_info("/proc/%s created\n", PROCFS_NAME); 
    return 0; 
} 
 
static void __exit My_proc_exit(void) 
{ 
    proc_remove(our_proc_file); 
    pr_info("/proc/%s removed\n", PROCFS_NAME); 
} 
 
module_init(My_proc_init); 
module_exit(My_proc_exit); 
 
MODULE_LICENSE("GPL");