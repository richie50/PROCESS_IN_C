/*
 * thread.c
 * Created June 11 , 2015
 * Author Richmond Frimpong
 * CSE23004 Student Number: 212114641
 * Multi Thread Program to read and process a numbers of files while outputting the sum , difference , minimum and the maximum of the file dataset
 * NOTE#: THIS PROGRAM HAS A SLEEP FUNCTION , PLEASE READ THE COMMENT AS TO WHY. THANK YOU.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

/*Make these global to have access to them in main*/
#define SIZE 1000000

struct results {
    char *file_nm;
    float sum;
    float diff;
    float min;
    float max;
};
struct results *data[SIZE];
int counter = 0;
int i = 0;
float all_data[SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //pthread initializer 
float maxi, mini; //overall min and max value

float max(float *data, int size) {
    int j = 0;
    float max = data[j];
    for (j = 0; j < size; j++) {
        if (data[j] > max) {
            max = data[j];
        }
    }
    return max;
}

float min(float *data, int size) {
    int j = 0;
    float min = data[j];
    for (j = 0; j < size; j++) {
        if (data[j] < min) {
            min = data[j];
        }
    }
    return min;
}
/*pthread runner function , process files, compute an provides the output*/

/* takes a file name from the command line */
void *runner(void *argv) {
    FILE *file;
    int scan = 0;
    float cur_max, cur_min;
    float max = 0.0;
    float min = 0.0;
    file = fopen(argv, "r");
	
    if (file == NULL) {
        fprintf(stderr, "Unable to read from file: %s\n", argv);
    }
    pthread_mutex_lock(&mutex); //lock data for thread specific , note max_data is global
    int init = 0;
    while (scan = fscanf(file, "%f", &cur_max) != EOF) {
        if (scan == 0) {
            fprintf(stderr, "Can't parse data in %s \n", argv);
            pthread_exit(NULL);
        }
        /* MIN , MAX , SUM , DIFF computation*/
        if (init == 0) {
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
    
    data[i] = (struct results*)malloc(sizeof(struct results));
    printf("I:%d\n" , i);
    data[i]->file_nm =  malloc(sizeof(char) * strlen(argv));
    strcpy(data[i]->file_nm , argv); 
    float sum = min + max;
    float diff = min - max;
    data[i]->sum = sum;
    data[i]->diff = diff;
    cur_min = (sum + diff) / (float) 2.0;
    cur_max = sum - cur_min;
    data[i]->min = cur_min;
    data[i]->max = cur_max;
    all_data[counter++] = cur_min;
    all_data[counter++] = cur_max;
	printf("%p\n" , data[i]);
    i++;
    pthread_mutex_unlock(&mutex); //release data for thread specific
    pthread_exit(&data);
}//end thread function

int main(int argc, char** argv) {
	
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t th[argc];
    pthread_t tid[argc];
    int thread_ret;
    long thread;
    char *data_args = malloc(sizeof (char)* 2048);
    struct results **temp;
    
   if (argc < 2) {
        fprintf(stderr, "Usage <program name><arguments . . . . . . .>\n");
        exit(1);
    }
    for (thread = 1; thread < argc ; thread++) {

        strncpy(data_args, argv[thread ], strlen(argv[thread]));
        tid[argc] = pthread_self();
        thread_ret = pthread_create(&th[thread], &attr, runner, (void *) data_args); /* execute thread with runner function */
	sleep(1); // better way is to use nanosleep function in c
        if (thread_ret < 0) {
            fprintf(stderr, "Thread with filename %s  %lu failed\n", data_args, (unsigned long) tid[thread]);
            break; //exit loop
        }
    }
    int k = 0;		
    for (thread = 1 ; thread < argc ; thread++) {
	//sleep(1); //i had to do this because the thread was not been safe even though lpthread is supposed to be safe.
	pthread_join(th[thread],(void**)&temp); // joins thread way too fast , inconsistent data
	//sleep(1); //i had to do this because the thread was not been safe even though lpthread is supposed to be safe.
	printf("Filename %s\tSUM=%f\tDIF=%f\tMIN=%f\tMAX=%f\n",argv[k+1], temp[k]->sum, temp[k]->diff, temp[k]->min, temp[k]->max);
	k++;
    }
    /*determining the overall minimum and maximum value by feeding the all current minimum and maximum through fucntion min and max */
    mini = min(all_data, counter);
    maxi = max(all_data, counter);
    printf("MINIMUM=%f\tMAXIMUM=%f\t\n", mini, maxi);
    pthread_mutex_destroy(&mutex); //destroy locks
    free(data_args);
    //free(data);
	pthread_exit(NULL);
    return (EXIT_SUCCESS);
}
