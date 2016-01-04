#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define LINE 256
 
void error_sys(const char *err){
	perror(err);
	exit(1);
}

int main(void){
	char buffer[LINE];
	pid_t process_id; //parent process upon which the child process is called
	int status;
	printf("%% ");//prompt
	while(fgets(buffer , LINE , stdin) != NULL){
		if(buffer[strlen(buffer) -1] == '\n'){
			buffer[strlen(buffer) -1] == '\0';
		}
		printf("cmd: %s \n" , buffer);
		 
		if(strncmp(buffer,"quit",4) == 0)
			break;
			//fork a new process check for errors		
		if((process_id = fork()) < 0){
			error_sys("Cant fork a new process");		
		}else if(process_id == 0){
			int wth = execv("/usr/ls/" , buffer); // executes a command
			printf("The process id for this is %ld %d \n" ,(long)getpid() , wth);
			error_sys("Couldn't execute");
			exit(127);
		}
		/* parent process */
		if((process_id = waitpid(process_id , &status , 0)) < 0);
			printf("Status :%d\n" , status);
			error_sys("Waitpid error");
			
			printf("%% ");
			printf("The process id for this is %ld \n" ,(long)getpid());
	}	
	exit(0);
}
