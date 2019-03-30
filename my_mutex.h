#ifndef __MY_MUTEX__
#define __MY_MUTEX__

// Implements a basic mutex class just for learning purpose.

class MyMutex {

  // Variable representing the state of the lock. If false, lock is available,
  // else, its not.
  bool lock_var = false;

  public:
  // Function to acquire the lock.
  void Acquire() {
    // Blocks until lock_var becomes false.
    while(__atomic_test_and_set(&lock_var, __ATOMIC_SEQ_CST));
  }

  // Function to release a lock.
  void Release() {
    lock_var = false;
  }

};
#endif // __MY_MUTEX__
