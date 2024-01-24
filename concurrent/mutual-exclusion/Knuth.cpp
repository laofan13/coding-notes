/* 
    @title: mutual exlusion
    @author: Knuth
    @paper: Additional Comments on a Problem in Concurrent Programming Control
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"

#define NLOOP 5000
#define N 5

int control[N],k;

int counter;

void doit(int i) {
    while(1) {
        control[i] = 1;
        int j = k;
        while(j != i) {
            if(control[j] != 0) {
                j = k;
            }
            j = (j - 1 + N) % N;
        }
        control[i] = 2;
        int faild = 0;
        for(int j = N - 1; j >= 0; --j) {
            if(j != i && control[j] == 2) {
                faild = 1;
                break;
            }
        }
        if(faild == 0) {
            break;
        }
    }
    // critical section
    k = i;
    int val = counter;
    printf("thread %d: %d\n", i, val + 1);
    counter = val + 1;
    k = i > 0 ? i - 1: N - 1;
    control[i] = 0;
}

void judge(int no) {
    printf("thread %d start tick!\n", no);
    for(int i = 0; i < NLOOP; ++i) {
        doit(no);
    }
    printf("thread %d exit\n", no);
}
 
int main(int argc, char **argv)
{
    pthread_t tids[N];
    memset(control, 0, sizeof(control));

    for(int i = 0;i < N; ++i) {
        pthread_create(&tids[i], NULL, judge, i);
    }
    /* wait for both threads to terminate */
    for(int i = 0;i < N; ++i) {
        pthread_join(tids[i], NULL);
    }


    printf("end counter = %d\n", counter);
 
    return 0;
}