#ifndef __PHILOSOPHER_SCHEDULER__
#define __PHILOSOPHER_SCHEDULER__

// Defines the Scheduler class.

#include<vector>
#include"../my_mutex.h"
#include"../my_semaphore.h"

// Forward declaration.
class Philosopher;

class PhilosopherScheduler {
  // Vector to store the current owner of the stick. -1 indicates no owner.
  std::vector<int> stick_owner;
  // Queue of philosophers waiting to obtain chopsticks.
  std::vector<Philosopher*> phil_queue;
  // Corresponding queue of eat signals.
  std::vector<MySemaphore*> eat_signal_queue;
  // Mutex lock to protect all above members.
  MyMutex mtx;

  public:
  PhilosopherScheduler(const int n_philosophers);

  void request_sticks(Philosopher* phil, MySemaphore* eat_signal);

  void release_sticks(Philosopher* phil);
};

#endif // __PHILOSOPHER_SCHEDULER__
