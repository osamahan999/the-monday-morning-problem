compile with 
 gcc -o stock stock.c -pthread -lpthread -lrt
run with ./stock


the monday morning problem:

Stock market opens with tons of queued up orders wanting to purchase a stock

Perhaps the stock price changes every 20 buy orders, then you would want to limit
how many stocks can purchase at base price

This program just simulates that with NUM_OF_THREADS amount of purchase orders at price BASE_STOCK_PRICE
and allows PURCHASE_QUEUE_LIMIT amount of purchases to go through before blocking the rest
Then, it changes the price by some amount, and allows the next amount of PURCHASE_QUEUE_LIMIT to purchase. 
The output gives the thread numbers and what prices they got.

This is similar to when you have a bunch of purchase orders with a max price, and it simulates how certain
threads may get the current stock price, but as the price increases, other threads will also get executed.

The reason why I decided to do it with threads and not with some kind of queue is because I don't think the 
NASDAQ or the NYSE utilizes a queue for the orders. I think that would not only take far too many resources
since as we saw, on october 12th, tesla had a volume of 39 million. Storing 39 million purchase orders would take
around 1 byte for the price, 4 bytes for date of order, and a few other bytes for the rest of the data, but suppose 
round to a total of 6 bytes per order, thats 234 million bytes, or 223 gigabytes just for the queue JUST for TSLA

Not only that, but locking the queue and adding the purchase orders onto it would take so long. Suppose the initial 6:30am volume
is 1 million total orders. Suppose the time it took to add to queue took 5ms per order, that's 5 million ms, or 16 minute and 44 seconds

Now obviously, the way that the NYSE would handle this would be a more 1-to-1 order kind of thing, rather than a set amount of orders per price.
AKA, the NYSE would take each sell order for a certain price, find enough buy orders to fullfil those sell orders, and have those go through.  
The market is a buyer-seller market, not some random number generator. For each purchase there is a seller, and that's how the prices would be 
calculated. 

All of this is really just speculation. I really am unsure how the NYSE would handle these hundreds of millions of orders per day. The numbers
are too big for me to wrap my head around. Would love to hear anyone else's take on this though, please feel free to reach out!

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
