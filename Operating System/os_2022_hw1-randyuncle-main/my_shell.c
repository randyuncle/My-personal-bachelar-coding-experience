#ifndef _POSIX_C_SOURCE
	#define  _POSIX_C_SOURCE 200809L
#endif

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUFFSIZE 64

int cmd_par_line = 50; /*the buffer size for a argument or a command*/
int MAX; /*global maximal (or be of the blocks for a current sliced command line)*/
int backgroung_sign; /*the signal for every cost*/
int in_pipe; /*the signal indicates that the process is in pipe*/

/**
 * the history linked list to save the arguments of last 16 command line
*/
typedef struct history * savepointer;
struct history{
	char **args;
	int max;
	savepointer next;
};
savepointer front = NULL, rear = NULL;
int saveBuff = 0; //the size of the history linked list

/**
 * the linked list to save the sliced command string blocks for pipeline issue
*/
struct pipecommand{
	char **args;
	int max; /*maximal of sliced command line*/
};
typedef struct pipecommand pipecommand;

/**
 * a command used to do the history update, 
*/
void historyUpdate(char **args){
	savepointer new = (savepointer)malloc(sizeof(struct history));

	new->args = malloc(sizeof(char*)*(MAX + 1));
	for(int i = 0 ; i < MAX ; i++){
		new->args[i] = malloc(sizeof(char)*cmd_par_line);
		strcpy(new->args[i], args[i]);
	}
	new->max = MAX;
	new->next = NULL;

	if(!front){
		front = new;
		if(!rear)
			rear = front;
	}else{
		front->next = new;
		front = front->next;
	}

	saveBuff++;
	if(saveBuff > 16){
		savepointer temp = rear;
		rear = rear->next;

		free(temp);
		saveBuff--;
	}
}

/**
 * reading the line
*/
char *read_line(){
	char *line; ssize_t buff = 0;

	if(getline(&line, &buff, stdin) < 0){
		printf("shell error : failed to readline\n");
		exit(1);
	}

	return line;
} 

/**
 * @brief showing the replay
 * 
 * @param args : 2D-array SINGLE command handler
 * @return savepointer : the command the user wants to replay
 */
savepointer sh_replay(char **args){
	//null check
	if(args[1] == NULL){
		printf("replay : no parameter input\n");
		return NULL;
	}

	//bound check
	int num = atoi(args[1]); //string -> int
	if(num < 1 || num > saveBuff){
		printf("replay : wrong args”\n");
		return NULL;
	}

	savepointer curr; 
	int i = 1;
	for(curr = rear ; curr != NULL ; curr = curr->next, i++){
		if(i == num)
			break;
	}
	
	return curr;
}

/**
 * parsing the commands
*/
char **split_line(char *line){
	int bufsize = BUFFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens){
		printf("shell error : there's no space to make new tokens\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \t\r\n\a");

	while(token){
		if(!strcmp(token, "replay")){ //identify replay -> do the replacement from history linked list
			char **temp = malloc(3 * sizeof(char*));
			temp[0] = token;
			token = strtok(NULL, " \t\r\n\a");
			temp[1] = token;

			savepointer local = sh_replay(temp);
			if(!local){
				free(tokens);
				free(temp);
				return NULL;
			}else{
				for(int i = 0 ; i < local->max; i++)
					tokens[position++] = local->args[i];
			}

			free(temp);
		}else{
			if(!strcmp(token, "&"))
				backgroung_sign++;

			tokens[position++] = token;

			if(position >= bufsize){
				bufsize += BUFFSIZE;
				tokens = realloc(tokens, bufsize*sizeof(char*));

				if(!tokens){
					printf("my_shell error : there's no space to make new tokens\n");
					exit(1);
				}
			}
		}

		token = strtok(NULL, " \t\r\n\a");
	}

	tokens[position] = NULL;
	MAX = position;
	return tokens;
}

/**
 * print the help message
*/
void sh_help(char **args){
	printf("-------------this is a help message of my_shell-------------\n");
	printf("My shell\n");
	printf("Type the program names and the arguments, then hit enter to execute.\n");
	printf("\n");
	printf("The followings are built in\n");
	printf("1: help:    show all built-in function info\n");
	printf("2: cd:      change directory\n");
	printf("3: echo:    echo the string to standard output\n");
	printf("4: record:  show last-16 cmds you typed in\n");
	printf("5: replay:  re-execute the cmd showed in record\n");
	printf("6: mypid:   find and print process-ids\n");
	printf("7: exit:    exit shell\n");
	printf("\n");
	printf("Use the \"man\" command for information on other programs\n");
	printf("-------------end of the help message of my_shell-------------\n");
}

/**
 * step to the directory the user wants
*/
void sh_cd(char **args){
	if (chdir(args[1]) != 0) 
		printf("cd : there's no directory path you want to go to\n");
}

/**
 * print the string to be echoed
*/
void sh_echo(char **args){
	if(!strcmp(args[1], "-n")){
		for(int i = 2 ; i < MAX; i++){
			printf("%s", args[i]);
			if((i + 1) != MAX) printf(" ");
		}
	}else{
		for(int i = 1 ; i < MAX; i++)
			printf("%s ", args[i]);
		printf("\n");
	}
}

/**
 * exit my_shell.c
*/
void sh_exit(){
	for( ; rear != NULL ;){
		savepointer temp = rear;
		rear = rear->next;

		free(temp);
	}
	printf("Goodbye ! See you next time !\n");
	exit(0);
}

/**
 * @brief printing the last 16 history commands
 * 
 * @param args 
 */
void sh_record(){
	if(!rear) return;

	savepointer curr; int i = 1;
	for(curr = rear ; curr != NULL ; curr = curr->next, i++){
		printf("%d: ", i);
		for(int j = 0 ; j < curr->max ; j++)
			printf("%s ", curr->args[j]);
		printf("\n");
	}
}

void sh_mypid(char **args){
	if(MAX == 1){
		printf("mypid : lack of the arguments\n");
		return;
	}else if(!strcmp(args[1], "-i")){
		pid_t pid = getpid();
		printf("%d\n", pid);
	}else if(!strcmp(args[1], "-p")){ //find parent
		int ppid;
		int pid = atoi(args[2]);
		FILE *fp;

		char input[32], buf[BUFSIZ];
		snprintf(input, sizeof(input), "/proc/%u/status", pid);
        fp = fopen(input, "r");
		
		if(fp){
			size_t ret = fread(buf, sizeof(char), (BUFSIZ*2)-1, fp);
			if (!ret){
				printf("myid -p: file read error\n");
				return;
			}else
				buf[ret++] = '\0';  // Terminate it.
		}else{
			printf("myid -p: process id not exist\n");
			return;
		}
		fclose(fp);

		char *ppid_loc = strstr(buf, "\nPPid:");
		if(ppid_loc){
			sscanf(ppid_loc, "\nPPid:%d", &ppid);
			if (!ppid || ppid == EOF)
				printf("myid -p: no parent process id\n");
			else
				printf("%d\n", ppid);
		}
	}else if(!strcmp(args[1], "-c")){ //find child
		int ppid;
		int pid = atoi(args[2]);
		FILE *fp;

		char input[32], buf[BUFSIZ];
		snprintf(input, sizeof(input), "/proc/%u/task/%u/children", pid, pid);
        fp = fopen(input, "r");
		if(fp){
			size_t ret = fread(buf, sizeof(char), (BUFSIZ*2)-1, fp);
			if (!ret){
				printf("myid -c: no child exists\n");
				return;
			}
			else
				buf[ret++] = '\0';  // Terminate it.
		}else{
			printf("myid -p: process id not exist\n");
			return;
		}
		fclose(fp);

		printf("%s\n", buf);
	}else
		printf("mypid : invalid argument \"%s\"\n", args[1]);
}

/**
 * @brief trigger the built-in commands in my_shell.c
 * 
 * @param args : 2D-array SINGLE command handler
 */
void shell_command_trigger(char **args){
	//shell build-in command
	if(!strcmp(args[0], "help"))
		sh_help(args);
	else if(!strcmp(args[0], "cd"))
		sh_cd(args);
	else if(!strcmp(args[0], "echo"))
		sh_echo(args);
	else if(!strcmp(args[0], "exit"))
		sh_exit();
	else if(!strcmp(args[0], "record"))
		sh_record();
	else if(!strcmp(args[0], "replay"))
		sh_replay(args);
	else if(!strcmp(args[0], "mypid"))
		sh_mypid(args);
	else{
		int status; pid_t pid, wpid;
		//command for linux
		pid = fork();

		if(pid < 0) //error forking
			perror("my_shell");
		else if(pid == 0){ //child process 
			if (execvp(args[0], args) == -1)
				perror("my_shell");

			exit(1);
		}else{ //parent process
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
			}while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
}

/**
 * @brief manage the command string with input ("<") command
 * 
 * @warning this command doesn't support the built-in commands
 */
void input_holder(char **args){
	char *input; int i;
	for(i = 0 ; i < MAX ; i++){
		if(!strcmp(args[i], "<")){
			if((i + 1) < MAX)
				input = args[i + 1];
			else{
				printf("input holder : lack of the input file name\n");
				return;
			}
			break;
		}
	}

	char **newArgs = malloc(BUFFSIZE * sizeof(char*));
	for(int j = 0 ; j < i ; j++)
		newArgs[j] = args[j];

	pid_t pid, wpid;  int status;
	pid = fork();
	if(pid < 0) //error forking
		perror("lsh");
	else if(pid == 0){ //child process
		int fd;
        fd = open(input, O_RDONLY);
		if(fd < 0) {
			printf("input holder : failing to open the file\n");
			exit(1);
		}
        dup2(fd, STDIN_FILENO);

		if(execvp(newArgs[0], newArgs) == -1)
			perror("my_shell");

		if(fd != STDIN_FILENO) 
			close(fd);

		exit(1);
	}else{ //parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		}while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

/**
 * @brief manage the command string with output (">") command
 * 
 * @param args 
 * @return int 
 */
void output_holder(char **args){
	char *output; int i;
	for(i = 0 ; i < MAX ; i++){
		if(!strcmp(args[i], ">")){
			if((i + 1) < MAX)
				output = args[i + 1];
			else{
				printf("output holder : lack of the output file name\n");
				return;
			}
			break;
		}
	}

	MAX = i;
	char **newArgs = malloc(BUFFSIZE * sizeof(char*));
	for(int j = 0 ; j < MAX ; j++)
		newArgs[j] = args[j];

	int input_sign = 0;
	//input holds
	for(int i = 0; i < MAX; i++) {
		if(!strcmp(args[i], "<")) {
			input_sign++;
			break;
		}
	}
	
	int status; pid_t pid, wpid;
	//command for linux
	pid = fork();

	if(pid < 0) //error forking
		perror("my_shell");
	else if(pid == 0){ //child process 
		//redirect to output file
		int fd;
		fd = open(output, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
		if(fd < 0) {
			printf("output holder : failing to open the file\n");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);

		//built-in command
		if (!strcmp(newArgs[0], "help") ||
			!strcmp(newArgs[0], "cd") ||
			!strcmp(newArgs[0], "echo") ||
			!strcmp(newArgs[0], "record") ||
			!strcmp(newArgs[0], "replay") ||
			!strcmp(newArgs[0], "mypid") ||
			!strcmp(newArgs[0], "exit")
		)
			shell_command_trigger(newArgs);
		else if(input_sign)
			//the command state built for the single process
			input_holder(newArgs);
		else{
			//system call
			if (execvp(newArgs[0], newArgs) == -1)
				perror("my_shell");
		}

		if (fd != STDOUT_FILENO) 
			close(fd);

		exit(1);
	}else{ //parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		}while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

/**
 * @brief doing the SINGLE process command
 * 
 * @param args 
 */
void process_execute(char **args){
	//output holds
	for(int i = 0; i < MAX; i++) {
        if(!strcmp(args[i], ">")) {
            output_holder(args);
            return;
        }
    }

    //input holds
    for(int i = 0; i < MAX; i++) {
        if(!strcmp(args[i], "<")) {
            input_holder(args);
            return;
        }
    }

	//nothing special -> do the normal command
	shell_command_trigger(args);
}

/**
 * doing the multi-process pipeline command
 * REMARK: call the parent iteratively
*/
void multi_process_execute(int process, char **args){
	in_pipe = 1;
	pipecommand pipcmd[process + 1]; int cnt = 0, args_start = 0;

	/*split the command to different pipe register*/
	for(int i = 0 ; i < MAX ; i++){
		int local_max = 0;
		if(!strcmp(args[i], "|")){
			pipcmd[cnt].args = malloc(sizeof(char*) * 64);
			for(int j = args_start ; j < i ; j++, local_max++)
				pipcmd[cnt].args[local_max] = args[j];
			pipcmd[cnt++].max = local_max; 
			args_start = i + 1;

			//last command -> do the specific split
			if(!(--process)){
				local_max = 0;
				pipcmd[cnt].args = malloc(sizeof(char*) * 64);
				for(int j = args_start ; j < MAX ; j++, local_max++)
					pipcmd[cnt].args[local_max] = args[j];
				pipcmd[cnt].max = local_max; 
				break;
			}
		}
	}

	pid_t pid;  int status;
	int pd[2];
	int prev_pip = STDIN_FILENO;/*the previous pd[0] status saving*/

	for(int i = 0 ; i <= cnt ; i++){
		MAX = pipcmd[i].max;

		/*create current pipeline*/
		if(pipe(pd) < 0) {
			perror("pipe()");
			exit(1);
		}

		pid = fork();
		if(pid < 0){ //error forking
			perror("my_shell");
			exit(1);
		}else if(pid == 0){ //child process
			dup2(prev_pip, STDIN_FILENO); //change the reading direction to previous method
			close(pd[0]); //closing the reading port that the child doesn't need

			/*checking if the pipe string is finish*/
			if((i + 1) <= cnt) 
				dup2(pd[1], STDOUT_FILENO);	
			process_execute(pipcmd[i].args);

			exit(1);
		}else{ //parent
			if(!backgroung_sign)
				//no "&"-> waiting
				wait(0);
			else{
				//"&" -> print the last process id 
				if(i == cnt)
					printf("[Pid] : %d\n", pid);
			}
			
			close(pd[1]);
			prev_pip = pd[0];
		}
	}
}

/**
 * this background is for the single process
*/
void background_holder(char **args) {
    pid_t pid;

    pid = fork();
    if(pid < 0){ 
		//error forking
        perror("my_shell");
		exit(1);
    }else if(pid == 0){
		//child
        process_execute(args);

        exit(1);
    }else{
		//parent
        printf("[Pid] : %d\n", pid);
	}
}

/**
 * identify and maintain the state of the input command(s)
*/
void sh_identify(char **args){
	if(args == NULL || args[0] == NULL) 
		return;
	
	//updating the current command line (or be called blocks now) to the linked list
	historyUpdate(args);

	if(backgroung_sign){
		MAX--;
		args[MAX] = NULL;
	} 

	/*calculate the number of pipes*/
	int process = 0;
	for(int i = 0 ; i < MAX ; i++){
		if(!strcmp(args[i], "|"))
			process++;
	}

	if(process > 0)
		multi_process_execute(process, args);
	else {
		if(backgroung_sign)
			background_holder(args);
		else
			process_execute(args);
	}
}

/**
 * maintain the shell loop
 */
void sh_command_adapt(){
	while(1){
		backgroung_sign = 0;
		in_pipe = 0;

		char *line, **args;
		printf(">>> $ ");
		
		line = read_line();
		args = split_line(line);
		sh_identify(args);

		free(line);
		free(args);
	}
}

int main(int argc, char **argv){
	printf("Welcome to program : my_shell !\nHave a nice Day~\n");
	sh_command_adapt();
	return 0;
}
