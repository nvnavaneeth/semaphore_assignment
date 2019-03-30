#ifndef __MY_SEMAPHORE__
#define __MY_SEMAPHORE__
// Implements a naive semaphore class purely for understanding purpose.

#include<mutex>
#include<queue>
#include<condition_variable>

class MySemaphore {
  
  // The semaphore variable.
  int count;
  // Mutex to protect CS.
  std::mutex mtx;
  // Queue to store conditional variables that can be used to wake up threads
  // put to sleep.
  std::queue<std::condition_variable*> cv_queue;

  public:
 
  // Constructor.
  MySemaphore(int initial_count);

  // Blocks until 'count' becomes a non-zero positive integer. Once unblocked,
  // decrements 'count'.
  void Wait();

  // Increments 'count'
  void Signal();

};

#endif // __MY_SEMAPHORE__
