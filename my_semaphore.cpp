#include "my_semaphore.h"
#include<iostream>

using namespace std;


MySemaphore::MySemaphore(int initial_count): count(initial_count) {
};


void MySemaphore::Wait() {

  // Obtain mutex to enter critical section.
  unique_lock<mutex> lck(mtx);

  if (count  == 0) {
    // Need to wait, put calling thread to sleep on a conditional variable. 
    condition_variable* cv = new condition_variable();
    // Add cv to queue to be later notified.
    cv_queue.push(cv);
    // Release mutex and wait for cv.
    cv->wait(lck);
    // Out of waiting state, which means some other thread has Signalled. 
    // Use of cv is over.
    delete cv;
    return;
  }

  count--;
  // Leaving CS.
  lck.unlock();
}


void MySemaphore::Signal() {
  unique_lock<mutex> lck(mtx);

  // If no other thread is waiting on the semaphore, just increase thecount 
  // variable.
  if(cv_queue.empty()) {
    count ++;
  } else {
    auto* cv = cv_queue.front();
    cv_queue.pop();
    // Wakeup the thread corresponding to this cv.
    cv->notify_one();
  }

  lck.unlock();
}
