#define _GNU_SOURCE
#define NUM_OF_THREADS 1000

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

/*
Monday morning Problem

Stock with cost 100 at time = 0

I'd like to simulate 1000 purchases of stock at same time. 


semaphore with x amount initialized, such that when that many orders go through, price changes, and no other orders can go through

execute order semaphore that once we reach the x amount, stops all other orders from going through 


readers-writers problem
rw_mutex to read and write
r_mutex for just read
read_count int to let rw_mutex know that there are readers

need protocol for handling reading concurently vs letting writers write


initialize 1000 threads and make them wait for a signal
use a cv




*/

//function declarations
void *threadFunc();

int threadCount = 0;

sem_t exeOrder;
pthread_mutex_t mutex;

int main()
{
    sem_init(&exeOrder, 0, 1);

    int i;
    pthread_t threadNum[NUM_OF_THREADS];

    for (i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_create(&threadNum[i], NULL, threadFunc, NULL);
    }

    for (i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_join(threadNum[i], NULL);
    }
}

void *threadFunc()
{

    //making all the threads wait until they're all ready
    pthread_mutex_lock(&mutex);
    threadCount++;
    pthread_mutex_unlock(&mutex);

    //all threads wait for count to get to 1000 before running the next code
    while (threadCount != 1000)
        ;
}
