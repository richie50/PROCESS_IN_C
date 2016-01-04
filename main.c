/* 
 * File:   main.c
 * Author: RichMond
 *
 * Created on June 13, 2015, 2:01 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * 
 */
float *array(float *ptr, int size) {
    ptr = malloc(sizeof (float) * size);
    int index = 0;
    for (index = 0; index < size; index++) {
        ptr[index] = (float) 0.0;
    }
    return ptr;
}

void max_min(float *data_array, int size) {
    int j = 0;
    float max = data_array[j];
    float min = data_array[j];
    for (j = 0; j < size; j++) {
        if (data_array[j] > max) {
            max = data_array[j];
        }
        if (data_array[j] < min) {
            min = data_array[j];
        }
        //printf("index:%d data=> %f\n", j, data_array[j]);
    }
    printf("MAX: %f  MIN: %f \n", max, min);
}

int main(int argc, char** argv) {
    pid_t pid[4];
    int status;
    int start = 0;
    while (start < 4) {
        pid[start] = fork();
        if (pid[start] < 0)
            exit(0);
        else if (pid[start] == 0) {
            FILE *file;
            char *data = malloc(sizeof (char) * 1024);
            char * tokens;
            float *data_array;
            int i = 0;
            const char space[2] = " ";
            float dynamic;
            file = fopen("data1", "r");
            if (file == NULL) {
                fprintf(stderr, "Cant open file\n");
            }
            while (fgets(data, 1024, file) != NULL) {
                tokens = strtok(data, space);
                while (tokens != NULL) {
                    sscanf(tokens, "%f", &dynamic);
                    data_array[i] = dynamic;
                    printf("%f\n" , data_array[i]);
                    tokens = strtok(NULL, space);
                    i++;
                }
            }
            fclose(file);
            printf("Child Process ID: %ld\n", getpid());
           // max_min(data_array, i);
        } else {
            sleep(1);
            wait(&status);
            printf("I'm the parent!\n");
	}
        start++;
    }
    return (EXIT_SUCCESS);
}

