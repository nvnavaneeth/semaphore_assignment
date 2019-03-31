#include"phil.h"
#include"sched_phil.h"
#include<thread>
#include<chrono>
#include<ctime>
#include<iostream>
#include"../my_semaphore.h"

using namespace std;

Philosopher::Philosopher(int id, int right_stick, int left_stick, 
    PhilosopherScheduler* scheduler) :
    scheduler(scheduler),
    id(id),
    right_stick(right_stick),
    left_stick(left_stick) {
}

Philosopher::~Philosopher() {
}

void Philosopher::run(bool* run_flag) {
  while(*run_flag) {
    // ----------- --------- Thinking state ----------------------------------
    // Think for a random amount of time.
    cout<<"P "<<id<<": Thinking\n";
    int think_time = rand()%2000;
    this_thread::sleep_for(chrono::milliseconds(think_time));
    //------------------------------------------------------------------------

    // ----------------------- Hungry state ----------------------------------
    cout<<"P "<<id<<": Hungry \n";
    // Semaphore with which scheduler indicates that both the chopsticks 
    // has been assigned.
    MySemaphore eat_signal(0);
    // Request required chopsticks from scheduler.
    scheduler->request_sticks(this, &eat_signal);
    // Wait for the eat signal.
    cout<<"P "<<id<<": Waiting for chopsticks \n";
    eat_signal.Wait();
    //------------------------------------------------------------------------

    // ------------------------ Eating state ---------------------------------
    // Eat for a random amount of time.
    cout<<"P "<<id<<": Obtained chopsticks, eating \n";
    int eat_time = rand()%1000;
    this_thread::sleep_for(chrono::milliseconds(eat_time));
    cout<<"P "<<id<<": Finished eating\n";
    // Relese the chopsticks.
    scheduler->release_sticks(this);
    //------------------------------------------------------------------------
  }
}
