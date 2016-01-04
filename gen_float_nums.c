/*
 *
 * A tester program to generate random floating point numbers
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(){
    srand((unsigned int)time(NULL));
    int i = 0;
    for(i = 0 ; i < 5 ; i++){
        printf("%f ", ((float)rand()/(float)(RAND_MAX)) * 99.0);
    }
    return 0;
}
