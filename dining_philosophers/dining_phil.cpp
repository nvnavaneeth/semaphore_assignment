#include<iostream>
#include<thread>
#include<stdlib.h>
#include<chrono>
#include<vector>
#include<ctime>
#include"../my_mutex.h"
#include"../my_semaphore.h"
#include"phil.h"
#include"sched_phil.h"

using namespace std;

const int N_PHILOSOPHERS = 5;

int main() {
  srand(time(0));
  cout<<"Here";

  PhilosopherScheduler scheduler(N_PHILOSOPHERS);

  cout<<"Here";
  vector<Philosopher> phil_vec;
  vector<thread> phil_thread_vec;

  cout<<"Here";
  // Initialize the philosophers. Philosophers are numbered from 1 to
  // N_PHILOSOPHERS. Sticks are numbered from 1 to N_PHILOSOPHERS in
  // clockwise manner, with  no. 1 assigned to the chopstick right of
  // philosopher no. 1.
  for (int i = 1; i <= N_PHILOSOPHERS; ++i) {
    phil_vec.emplace_back(i, i, (i+1)%N_PHILOSOPHERS, &scheduler); 
  }

  cout<<"Phils created\n";
  bool run_flag = true;
  bool *run_flag_ptr = &run_flag;

  for (int i = 1; i <= N_PHILOSOPHERS; ++i) {
  //  phil_thread_vec.emplace_back(&Philosopher::run, &phil_vec[i-1], run_flag_ptr);
  }

  cout<<"threads created\n";
  // Let philosopher threads run for some time.
  this_thread::sleep_for(chrono::seconds(5));
  run_flag = false;

  for (auto& phil_thread : phil_thread_vec) {
    phil_thread.join();
  }
}

