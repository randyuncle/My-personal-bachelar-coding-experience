#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>

#define gettid() ((pid_t)syscall(SYS_gettid))/*system call for getting the thread id*/

#define FILE_NAME_SIZE 20 /*the input file name buffer*/
#define PROC_FILE_NAME "/proc/thread_info"
#define OUTPUT_FILE "result.txt"

pthread_mutex_t mutex; /*for pthread locking and unlocking*/
int proc_id = 0; /*the id for each thread process execute*/

/*the input matrix properties*/
int ra, ca, rb, cb; /*if ca == rb -> legal matrix multiplication*/
long long int **matrixA, **matrixB;
int threads[128];

/*the output matrix properties*/
int rout, cout;
long long int **out_matrix;

/*this data type represents the matrix position that the local of each threads will do the calculation*/
typedef struct local_matrix{
    int r_start; /*local starting row*/
    int c_start; /*local starting colemn*/
    int r_end; /*local ending row*/
    int c_end; /*local ending colemn*/
}local_matrix;
local_matrix *locals;

/**
 * initializing the matrixA
*/
void init_matrixA(char* input){
    char buff[BUFSIZ];
    FILE *fp;

    /* file I/O */
    fp = fopen(input, "r");
    if(!fp){
        printf("init_matrix_from_file : input file doesn't exist\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d %d", &ra, &ca);

    /*matrix initialization*/
    matrixA = (long long int**)malloc(sizeof(long long int*) * ra);
    for(int i = 0 ; i < ra ; i++)
        matrixA[i] = (long long int*)malloc(sizeof(long long int) * ca);

    for(int i = 0 ; i < ra ; i++)
        for(int j = 0 ; j < ca ; j++)
            fscanf(fp, "%lld", &matrixA[i][j]);

    fclose(fp);
}

/**
 * initializing the matrixB
*/
void init_matrixB(char* input){
    char buff[BUFSIZ];
    FILE *fp;

    /* file I/O */
    fp = fopen(input, "r");
    if(!fp){
        printf("init_matrix_from_file : input file doesn't exist\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d %d", &rb, &cb);

    /*matrix initialization*/
    matrixB = (long long int**)malloc(sizeof(long long int*) * rb);
    for(int i = 0 ; i < rb ; i++)
        matrixB[i] = (long long int*)malloc(sizeof(long long int) * cb);

    for(int i = 0 ; i < rb ; i++)
        for(int j = 0 ; j < cb ; j++)
            fscanf(fp, "%lld", &matrixB[i][j]);

    fclose(fp);
}

/**
 * initializing the out_matrix
*/
void init_output_matrix(){
    out_matrix = (long long int**)malloc(sizeof(long long int*) * rout);
    for(int i = 0 ; i < rout ; i++)
        out_matrix[i] = (long long int*)malloc(sizeof(long long int) * cout);
}

/**
 * output the out_matrix to the specific output file (result.txt)
*/
void out_matrix_to_file(){
    FILE *fp;

    fp = fopen(OUTPUT_FILE, "w");
    if(!fp){
        printf("out_matrix_to_file : couldn't generate the output file \n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%d %d\n", rout, cout);
    for(int i = 0 ; i < rout ; i++){
        for(int j = 0 ; j < cout ; j++)
            fprintf(fp, "%lld ", out_matrix[i][j]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/**
 * doing the /proc/thread_info content writing (user -> kernel)
*/
void thread_info_write(){
    int fd, sz;

    fd = open(PROC_FILE_NAME, O_WRONLY);
    if(fd < 0){
        printf("open /proc/thread_info file error\n");
        exit(EXIT_FAILURE);
    }

    pid_t tid = gettid();/*get the thread id (in linux procession)*/
    char tid_str[20];
    sprintf(tid_str, "%ld", (long int)tid);/*changing the result to a string*/
    sz = write(fd, tid_str, strlen(tid_str));/*send the message into the /proc/thread_info file (user -> kernel)*/
    if(sz < 0){
        printf("cannot write thread id into the /proc file\n");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

/**
 * reading the /proc/thread_info file (kernel -> user)
*/
void thread_info_read(){
    int fd, sz;

    fd = open(PROC_FILE_NAME, O_RDONLY);
    if(fd < 0){
        printf("open /proc/thread_info file error\n");
        exit(EXIT_FAILURE);
    }

    /*getting the threads informations*/
    char buf[1024];
    sz = read(fd, &buf, 1024);
    if(sz < 0){
        printf("cannot read the /proc file\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();
    printf("PID:%d\n", (int)pid);

    /*printing the threads information*/
    char *token; 
    int counter = 0;/*the printing state of the tokens*/
    token = strtok(buf, " \t\r\n\a");
    while(token != NULL){
        if(counter > 2) counter = 0;

        int num = atoi(token);
        if(!counter)
            printf("    ThreadID:%d ", num);
        else if(counter == 1)
            printf("Time:%d(ms) ", num);
        else
            printf("context switch time:%d\n", num);
        
        token = strtok(NULL, " \t\r\n\a");
        counter++;
    }
}

/**
 * @remark :[GLOBAL]
 * matrix multiplication function (O(n^3))
*/
void *thread_mat_mul(void *arg){
    int local_id;

    /*prevent from the race condition and critical section*/
    pthread_mutex_lock(&mutex);
    local_id = proc_id++;
    pthread_mutex_unlock(&mutex);

    int r_start = locals[local_id].r_start;
    int r_end = locals[local_id].r_end;
    int c_start = locals[local_id].c_start;
    int c_end = locals[local_id].c_end;

    /*matrix multiplication*/
    int i, j, j_s, j_e, k;
    for(i = r_start ; i <= r_end ; i++){
        /*define the current starting and ending colemn (of the out_matrix)*/
        if(i == r_start){
            /*the start of j must be c_start*/
            j_s = c_start;
            if(r_start == r_end)/*if the r_end is in the same row of r_start*/
                j_e = c_end;
            else
                j_e = cout - 1;
        }else if(i == r_end){
            /*the last row of this thread*/
            j_s = 0;
            j_e = c_end;
        }else{
            /*not hitting the end of the row*/
            j_s = 0;
            j_e = cout - 1;
        }

        for(j = j_s; j <= j_e ; j++){
            out_matrix[i][j] = 0;
            for(k = 0 ; k < ca ; k++)
                out_matrix[i][j] += (matrixA[i][k] * matrixB[k][j]);
        }
    }

    pthread_mutex_lock(&mutex);
    thread_info_write(); /* write the thread_id into the /proc/thread_info */
    pthread_mutex_unlock(&mutex);
}

/**
 * this function is used to split the elements of the result function to the local of each threads
*/
void split_matrix(int threads, int r, int c){
    /*initialization*/
    locals = (local_matrix *)malloc(sizeof(local_matrix) * threads);
    
    int cut = (r * c) / threads, id = 0, counter = 0;
    for(int i = 0 ; i < r ; i++){
        for(int j = 0 ; j < c ; j++){
            if(counter == 0){
                locals[id].r_start = i;
                locals[id].c_start = j;
            }

            counter++;

            if(counter == cut && id < (threads - 1)){
                locals[id].r_end = i;
                locals[id++].c_end = j;

                counter = 0;
            }
        }
    }

    locals[id].r_end = r - 1;
    locals[id].c_end = c - 1;
}


void execution(int threads, char* inputa, char *inputb){
    /*initializing the input matrices*/
    init_matrixA(inputa);
    init_matrixB(inputb);

    /*initialize the output matrix*/
    rout = ra;
    cout = cb;
    init_output_matrix();

    split_matrix(threads, rout, cout);

    /*thread starting*/
    int *data, i;
    pthread_t p[threads];
    pthread_mutex_init(&mutex, NULL);

    clock_t begin, end;
    begin = clock();
    for(i = 0 ; i < threads ; i++)
        pthread_create(&p[i], NULL, thread_mat_mul, (void*)data);
    for (i = 0 ; i < threads ; i++)
        pthread_join(p[i], NULL);   
    end = clock();
    //printf("The execution time of %d threads: %f(s)\n", threads, (double)(end - begin) / CLOCKS_PER_SEC);

    pthread_mutex_destroy(&mutex);
    thread_info_read();
    out_matrix_to_file();
    return;
}

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));
    char *inputa, *inputb;
    int threads;

    /**
     * argv[1]-> (int) thread number
     * argv[2]-> (char*) input file name
     * argv[3]-> (char*) output file name
    */
    threads = atoi(argv[1]);
    inputa = (char *)malloc(sizeof(char) * FILE_NAME_SIZE);
    inputb = (char *)malloc(sizeof(char) * FILE_NAME_SIZE);
    
    strcpy(inputa, argv[2]);
    strcpy(inputb, argv[3]);

    //go to the main implementation
    execution(threads, inputa, inputb);

    //free the memories
    free(inputa);
    free(inputb);
    return 0;
}