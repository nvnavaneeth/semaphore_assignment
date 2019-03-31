#include"sched_phil.h"
#include"phil.h"


PhilosopherScheduler::PhilosopherScheduler(const int n_philosophers):
    stick_owner(n_philosophers, -1) {
}


void PhilosopherScheduler::request_sticks(Philosopher* phil,
    MySemaphore* eat_signal) {
  mtx.Acquire();
  // Acquire left stick if its free.
  if (stick_owner[phil->left_stick]<0)
    stick_owner[phil->left_stick] = phil->id;
  // Acquire right stick if its free.
  if (stick_owner[phil->right_stick]<0)
    stick_owner[phil->right_stick] = phil->id;
  
  // If both sticks are obtained, let this philosopher know that he can eat.
  if (stick_owner[phil->left_stick] == phil->id &&
      stick_owner[phil->right_stick] == phil->id) {
    eat_signal->Signal();
  }
  else {
    // Put this philosopher in a queue to wait till chopsticks are available.
    phil_queue.push_back(phil);
    eat_signal_queue.push_back(eat_signal);
  }
  mtx.Release();
}


void PhilosopherScheduler::release_sticks(Philosopher* phil) {
  mtx.Acquire();
  stick_owner[phil->left_stick] = -1;
  stick_owner[phil->right_stick] = -1;
  
  for(int i = 1; i<= phil_queue.size(); ++i) {
    auto* phil = phil_queue[i];
    // Acquire left stick if its free.
    if (stick_owner[phil->left_stick]<0)
      stick_owner[phil->left_stick] = phil->id;
    // Acquire right stick if its free.
    if (stick_owner[phil->right_stick]<0)
      stick_owner[phil->right_stick] = phil->id;
    
    // If both sticks are obtained, remove him from the queue and him know
    // that he can eat.
    if (stick_owner[phil->left_stick] == phil->id &&
        stick_owner[phil->right_stick] == phil->id) {
      eat_signal_queue[i]->Signal();

      phil_queue.erase(phil_queue.begin() + i);
      eat_signal_queue.erase(eat_signal_queue.begin() + i);
    }
  }
  mtx.Release();
}
