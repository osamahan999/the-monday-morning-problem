#define _GNU_SOURCE
#define NUM_OF_THREADS 1000
#define BASE_STOCK_PRICE 200
#define PURCHASE_QUEUE_LIMIT 20

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

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

    if (queuePointer == PURCHASE_QUEUE_LIMIT)
    {
        stockPrice *= 1.01; //queue full, which means we're incrementing cost of stock
        sem_post(&full);    //posts to full that the last stuck threads can come through and change price of stock
    }

    else
    {
        queuePointer++;  //increment queue
        sem_post(&full); //not full

        sem_wait(&boundedBuffer); //lock buffer now
        printf("Holy shit im in!\n");

        // purchaseQueue[queuePointer] = pthread_self();
        sem_post(&boundedBuffer); //unlock
    }

    pthread_exit(0);
}