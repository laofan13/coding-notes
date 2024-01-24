/* 
    @title: mutual exlusion
    @author: Lamport
    @paper: A New Solution of Dijkstra’s Concurrent Programming Problem
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"

#define NLOOP 5000
#define N 5

int choosing[N], number[N];

int counter; 

void doit(int i) {
    choosing[i] = 1;
    int max = -1;
    for(int j = 0;j < N; ++j) {
        if(number[j] > max) {
            max = number[j];
        }
    }
    number[i] = 1 + max;
    choosing[i] = 0;

    for(int j = 0; j < N; ++j) {
        while(choosing[j] != 0);
        while(number[j] != 0) {
            if(number[j] > number[i] || (number[j] == number[i] && j >= i))
                break;
        }
    }

    // critical section
    int val = counter;
    printf("thread %d: %d\n", i, val + 1);
    counter = val + 1;
    number[i] = 0;
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
    memset(choosing,0,sizeof(choosing));
    memset(number,0,sizeof(number));

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