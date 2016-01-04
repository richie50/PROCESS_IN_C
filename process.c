/*
 * File:   process.c
 * Author: RICHMOND FRIMPONG
 * CSE23004 
 * 211214641
 * Created on June 10, 2015, 9:24 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <sys/types.h>

/*
 * Multiple process to handle files and compute sum , diff , min , max 
 * and pipe the results from the child process to standard output in main process
 */
#define BROKEN_PIPE_FLAG -2

int main(int argc, char** argv) {
	char output[2048];
	int i = 0;
	float sum = 0.0;
	float diff = 0.0;
	int file_handler = 0;	
	if (argc < 2) {
		fprintf(stderr, "Usage <program name> <filenames . . . .>\n");
		exit(1);
	}
	//array of pipes ,0 for reading , 1 for write
	int fd[argc - 1][2]; //multiple pipes
	pid_t process_id; //  process identifier
	for (i = 0; i < (argc - 1); i++) {
		/*Handle pipe errors for each process created*/
		if (pipe(fd[i]) < 0) {
			fd[i][0] = BROKEN_PIPE_FLAG;
			fd[i][1] = BROKEN_PIPE_FLAG;
			fprintf(stderr, "Pipe failed on %s \n", argv[i + 1]);
			exit(1);
		} else {
			process_id = fork();
			if (process_id < 0) {
				/*close pipes since they already open and there is a fork we must close them*/
				close(fd[i][0]); //IN
				close(fd[i][1]); //OUT
				fd[i][0] = BROKEN_PIPE_FLAG;
				fd[i][1] = BROKEN_PIPE_FLAG;
				perror("Could not create a process");
			} else if (process_id == 0) { /*child process*/
				float cur_max = 0.0;
                                float cur_min = 0.0;
				float max = 0.0;
				float min = 0.0;
				FILE *file;
				int tokens;
				file = fopen(argv[i + 1], "r");
				if (file == NULL) {
					fprintf(stderr, "Unable to read from file: %s\n", argv[i + 1]);
				}
                                int init = 0;
				while ((tokens = fscanf(file, "%f", &cur_max)) != EOF && errno == 0) { 
					if (tokens == 0) {
						fprintf(stderr, "Error data can not be parse in file %s\n", argv[i + 1]);
						break; 
					}
			/* COMPUTING THE CURRENT MIN AND MAX VALUES*/
                                        if(init == 0){
                                            max = cur_max;
                                            min = cur_max;
                                        }
					if (cur_max > max) {
						max = cur_max;
					}
					if (cur_max < min) {
						min = cur_max;
					}
                                        init++;
				}
				fclose(file); //close the file
				sum = (float)(min + max);
				diff = (float)(min - max);
				sprintf(output, "%f\t%f", sum, diff);
				/*open pipe channels*/
				close(fd[i][0]); //close the reading channel 
				if (write(fd[i][1], output, sizeof (output)) < 0) {
					perror("Cant write , Write channel failed");
				}
				exit(0);

			} else if (process_id > 0) { /*parent process*/
			}

		}/*working pipe*/
	}/*end loop*/
	/*main process continuation*/
	wait(NULL); /*wait for child process to */
	int j = 0;
	int n = 0;
	float cur_min = 0.0;
	float cur_max = 0.0;
	float max =0.0; 
	float min =0.0;
	for (i = 0; i < (argc - 1); i++) {
			close(fd[i][1]); //close writing subroutine
			n = read(fd[i][0], output, sizeof (output)); /* get the value from array pipes*/
			n = sscanf(output, "%f\t%f", &sum, &diff); /* gurantee that numbers coming the pipe is valid*/
			close(fd[i][0]); //close reading subroutine
			cur_min = (float)(sum + diff) / (float)2.0;
			cur_max = (float)sum - cur_min;
			printf("Filename %s\tSUM=%f\tDIFF=%f\tMIN=%f\tMAX=%f\n", argv[i + 1], sum, diff, cur_min, cur_max);
		if( i == 0){ /* set a pivot to compare current values , otherwise min and max are set to the current min and max values*/
			max = cur_max;
			min = cur_min;
		}
		if(cur_max > max){
			max = cur_max;
		}
		if(min > cur_min)
			min = cur_min;
	}/*end loop*/
	printf("MINIMIUM=%f\tMAXIMUM=%f\n", min , max);
	return (EXIT_SUCCESS);
}
