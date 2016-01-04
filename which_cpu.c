/* Short test program to test the pthread_setaffinity_np
 * (which sets the affinity of threads to processors).
 * Compile: gcc pthread_setaffinity_np_test.c
 *              -o pthread_setaffinity_np_test -lm -lpthread
 * Usage: ./pthread_setaffinity_test
 *
 * Open a "top"-window at the same time and see all the work
 * being done on CPU 0 first and after a short wait on CPU 1.
 * Repeat with different numbers to make sure, it is not a
 * coincidence.
 */
 
#include <stdio.h>
#include <math.h>
#include <pthread.h>
 
double waste_time(long n)
{
    double res = 0;
    long i = 0;
    while (i < n * 200000) {
        i++;
        res += i;
    }
    return res;
}
 
void *thread_func(void *param)
{
    unsigned long mask = 1; /* processor 0 */
	unsigned long core;
    /* bind process to processor 0 */
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask),
        &mask) < 0) {
        perror("pthread_setaffinity_np");
    }
	core = pthread_getaffinity_np(pthread_self(), sizeof(mask),&mask);
	printf("CPU Core %ld\t" , core);
    /* waste some time so the work is visible with "top" */
    printf("result: %f\n", waste_time(2000));
 
    mask = 2;   /* process switches to processor 1 now */
    if (core = pthread_setaffinity_np(pthread_self(), sizeof(mask),
        &mask) < 0) {
        perror("pthread_setaffinity_np");
    }
	core = pthread_getaffinity_np(pthread_self(), sizeof(mask),&mask);
	printf("CPU Core %ld\t" , core);
    /* waste some more time to see the processor switch */
    printf("result: %f\n", waste_time(200));
}
 
int main(int argc, char *argv[])
{
    pthread_t my_thread;
 
    if (pthread_create(&my_thread, NULL, thread_func,
        NULL) != 0) {
        perror("pthread_create");
    }
    pthread_exit(NULL);
}