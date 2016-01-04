#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

/****
NOTE:
the program will detect and report (to stderr) non-existent files
the program will detect and report (to stderr) files that don't contain correct data
the program will still compute on the valid remaining input
***/


#define IGNORE_PIPE_DUE_TO_PROBLEMS -1		// a flag to indicate pipes to ignore 

/*** main program logic ***/
int main(int argc, char** argv) {

	/* check command-line arguments */
	if (argc == 1) {
		fprintf(stderr, "usage: %s <filename1> <filename2> ...\n", argv[0]);
		exit(0);
	}

	/* working wariables and array of pipes */
	int i, n, count = argc - 1;	// auxiliary counter, argument count
	int fcp;					// intermediary count
	double fsp;					// intermediary sum
	int ipc[count][2];			// array of pipes
	
	/* for each command-line argument */
	for (i = 0; i < count; i++) {

		/* open a pipe for a child process */
		if (pipe(ipc[i]) < 0 ) {
			fprintf(stderr, "pipe error for file %s\n", argv[i+1]);
			ipc[i][0] = IGNORE_PIPE_DUE_TO_PROBLEMS;
			ipc[i][1] = IGNORE_PIPE_DUE_TO_PROBLEMS;
		}
		
		/* we have a good pipe, let's fork a child process */
		else {
			pid_t pid = fork();
	
			/*** bad fork ***/
			if (pid < 0) {
				/* close ends of pipe and indicate failure (=-1) */
				close(ipc[i][0]);	// release IN end
				close(ipc[i][1]);	// release OUT end
				ipc[i][0] = IGNORE_PIPE_DUE_TO_PROBLEMS;
				ipc[i][1] = IGNORE_PIPE_DUE_TO_PROBLEMS;
				fprintf(stderr, "error in fork\n");
			} else

			/*** parent ***/
			if (pid > 0) {
				/* just close OUT end of pipe then move on with the loop */
				close(ipc[i][1]);
			}
	
			/*** child ***/
			else if (pid == 0) {
				FILE *fh;
				int fc = 0;
				double fs = 0., x;
				
				/* close IN end of pipe and redirect STDOUT to OUT end of pipe */
				close(ipc[i][0]);	// release IN end
				dup2(ipc[i][1], 1);	// rediredt STDOUT to OUT end
				close(ipc[i][1]);	// release OUT end
	
				/* open and scan a file summing/counting double numbers until the end of file or error */
				if (fh = fopen(argv[i+1], "r")) {
					errno = 0;
					while ((n = fscanf(fh, "%lf", &x)) != EOF && errno == 0) {
					/* is read data not a real number then no error but parsed 0 arguments */
					if (n == 0) {
							fprintf(stderr, "error in file: %s\n", argv[i+1]);
							break;
						}
						fs += x;
						printf("here : %lf\n" , fs);
						fc++;
					}
					/* error reading the file */
					if (errno)
						perror(argv[i+1]);
					fclose(fh);
				} 

				/* error opening the file */
				else
					perror(argv[i+1]);
				
				/* write results to STDOUT (= OUT end of pipe) */
				if (fc != 0)
					printf("which one %lf %d 0", fs / fc, fc);
				else
					printf("%lf %d 0", 0., 0);
	
				/* child process processing done - terminate */	
				exit(0);
			}
	
			/* end of GOOD PIPE */
		}
		/* end of FOR LOOP body */
	}

	/* make sure all child processes finished */
	wait();

	/* collect results from valid pipes */
	int fct = 0;			// total count
	double fmp, fst = 0.;	// file mean, total sum

	for (i = 0; i < count; i++) {
		if (ipc[i][0] != IGNORE_PIPE_DUE_TO_PROBLEMS) {
			dup2(ipc[i][0], 0); // rediredt IN end of pipe to STDIN
			close(ipc[i][0]);	// release IN end of the pipe
			if (scanf("%lf %d", &fmp, &fcp) != EOF) {
				fst += fmp * fcp;
				fct += fcp;
			}
		}
	}

	/* write results, end of program */
	if (fct != 0)  
		printf("%f\n", fst / fct);
	else
		printf("%f\n", 0.);
	exit(0);
}
