#define _GNU_SOURCE
#define NUM_OF_THREADS 21
#define BASE_STOCK_PRICE 200
#define PURCHASE_QUEUE_LIMIT 3

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

//function declarations
void *threadFunc();
void purchaseStock();

int threadCount = 0; //used to get all threads to the same program counter
double stockPrice = BASE_STOCK_PRICE;

int queuePointer = 0; //keeps track of amt of purchases being done with current stock price

pthread_mutex_t mutex; //used to keep track of all threads to get them to the same program counter
sem_t full;            //used to see if the purchase queue is full & should change price
sem_t boundedBuffer;   //used when writing to allPurchases array

//data structure for just holding this data
typedef struct
{
    double stockPrice;
    pthread_t threadNumber;

} purchases;

purchases *allPurchases[NUM_OF_THREADS];
int purchasePointer = 0; //keeps track of index in array

int main()
{

    pthread_t threadNum[NUM_OF_THREADS];

    sem_init(&full, 0, 1); //initialized to 1 because I want to alternate between 1 and 0 for the lock.
    //Should probably start it at 10 and then reset it to 10 each time stock price changes tho

    sem_init(&boundedBuffer, 0, 1); //initialized to 1 b/c just used as a lock

    //creates all the threads
    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_create(&threadNum[i], NULL, threadFunc, NULL);
    }

    //joins all the threads
    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_join(threadNum[i], NULL);
    }

    //prints all the stock prices for the purchases
    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        printf("$%f  ---- threadNum: %d\n ", allPurchases[i]->stockPrice, (int)allPurchases[i]->threadNumber);
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

    purchaseStock();
}

void purchaseStock()
{
    // if full, wait
    sem_wait(&full);

    if (queuePointer == PURCHASE_QUEUE_LIMIT)
    {
        stockPrice *= 1.02; //queue full, which means we're incrementing cost of stock
        queuePointer = 0;
    }

    queuePointer++; //increment queue

    double temp = stockPrice; //gets price now so that we dont need a mutex for the price
    sem_post(&full);          //not full

    sem_wait(&boundedBuffer); //lock buffer now

    //has to be done in cs because dont want pointer to be reused
    allPurchases[purchasePointer] = malloc(sizeof(purchases));
    allPurchases[purchasePointer]->stockPrice = temp;
    allPurchases[purchasePointer]->threadNumber = pthread_self();

    purchasePointer++;

    sem_post(&boundedBuffer); //unlock

    pthread_exit(0); //kills threads who purchased
}