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

sample output for NUM_OF_THREADS 21 and PURCHASE_QUEUE_LIMIT 3

$200.000000  ---- threadNum: 8759040
 $216.486432  ---- threadNum: 176613120
 $204.000000  ---- threadNum: 159827712
 $200.000000  ---- threadNum: 143042304
 $204.000000  ---- threadNum: 42329856
 $225.232484  ---- threadNum: 17151744
 $204.000000  ---- threadNum: 151435008
 $208.080000  ---- threadNum: 75900672
 $208.080000  ---- threadNum: 59115264
 $208.080000  ---- threadNum: 67507968
 $212.241600  ---- threadNum: 168220416
 $212.241600  ---- threadNum: 50722560
 $212.241600  ---- threadNum: 117864192
 $216.486432  ---- threadNum: 33937152
 $220.816161  ---- threadNum: 126256896
 $216.486432  ---- threadNum: 109471488
 $220.816161  ---- threadNum: 101078784
 $225.232484  ---- threadNum: 84293376
 $225.232484  ---- threadNum: 25544448
 $200.000000  ---- threadNum: 134649600
 $220.816161  ---- threadNum: 92686080
