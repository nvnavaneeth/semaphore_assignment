#ifndef __PHILOSOPHER__
#define __PHILOSOPHER__
// Class defining a philosopher.

#include<memory>

// Forward declarations.
class PhilosopherScheduler;

class Philosopher {
  // Pointer to the Scheduler which assigns chopsticks.
  PhilosopherScheduler* scheduler;

  public:
  // Unique Id of the philosopher.
  int id;
  // Id of chopstick to left.
  int left_stick;
  // Id of chopstick to right.
  int right_stick;

  // Constructor.
  Philosopher(int id, int right_stick, int left_stick,
      PhilosopherScheduler* scheduler);
  ~Philosopher();

  void run(bool* run_flag);
};

#endif // __PHILOSOPHER__
