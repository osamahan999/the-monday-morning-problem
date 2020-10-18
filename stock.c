#define _GNU_SOURCE
#define NUM_OF_THREADS 10
#define BASE_STOCK_PRICE 200
#define PURCHASE_QUEUE_LIMIT 2

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
void purchaseStock();

int threadCount = 0; //used to get all threads to the same program counter
double stockPrice = BASE_STOCK_PRICE;

int purchaseQueue[PURCHASE_QUEUE_LIMIT];
int queuePointer = 0;

pthread_mutex_t mutex;
sem_t full;
sem_t boundedBuffer;

int main()
{

    pthread_t threadNum[NUM_OF_THREADS];

    sem_init(&full, 0, 1);
    sem_init(&boundedBuffer, 0, 1);

    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_create(&threadNum[i], NULL, threadFunc, NULL);
    }

    for (int i = 0; i < NUM_OF_THREADS; i++)
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
    while (threadCount != NUM_OF_THREADS)
        ;

    printf("%d threadcount rn\n", threadCount);

    purchaseStock();
}

void purchaseStock()
{
    // if full, wait
    sem_wait(&full);

    if (queuePointer == PURCHASE_QUEUE_LIMIT - 1)
    {
        stockPrice *= 1.01; //queue full, which means we're incrementing cost of stock
    }

    else
    {
        sem_post(&full); //not full

        sem_wait(&boundedBuffer); //lock buffer now
        printf("Holy shit im in!\n");

        // purchaseQueue[queuePointer] = pthread_self();
        queuePointer++;
        sem_post(&boundedBuffer); //unlock
    }

    //if writing to buffer alrdy, wait
}