compile with 
 gcc -o stock stock.c -pthread -lpthread -lrt
run with ./stock


the monday morning problem:

Stock market opens with tons of queued up orders wanting to purchase a stock

Perhaps the stock price changes every 20 buy orders, then you would want to limit
how many stocks can purchase at base price

This program just simulates that with NUM_OF_THREADS amount of purchase orders at price BASE_STOCK_PRICE
and allows PURCHASE_QUEUE_LIMIT amount of purchases to go through before blocking the rest

I used two semaphores and a mutex to do this. The mutex is used to simply lock the threadCount to make sure
that the program counter of each thread is at the exact same spot initially to simulate the 6:30 am conditions

The two semaphores are then used to check to see if the buffer is full AKA all threads that can purchase at the base
price have already placed their orders, and the boundedBuffer semaphore is used just for critical section writing in the array