/* 
    @title: mutual exlusion
    @author: Dijkstra
    @paper: Solution of a Problem in Concurrent Programming Control
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"

#define NLOOP 5000
#define N 5

int selected[N], control[N], k;

int counter;

void doit(int i) {
    selected[i] = 1;
    while(1) {
        if(k != i) {
            control[i] = 0;
            if (selected[k] == 1) {
                continue;
            }
        }
        k = i;
        control[i] = 1;
        int faild = 0;
        for(int j = 0; j < N; ++j) {
            if(j != i && control[j] == 1) {
                faild = 1;
                break;
            }
        }
        if(!faild)
           break;
    }
    // critical section
    int val = counter;
    printf("thread %d: %d\n", i, val + 1);
    counter = val + 1;
    selected[i] = 0;
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
    memset(selected,0,sizeof(selected));
    memset(control,0,sizeof(control));

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