#include"phil.h"
#include"sched_phil.h"
#include<thread>
#include<chrono>
#include<ctime>
#include<iostream>

using namespace std;

Philosopher::Philosopher(int id, int right_stick, int left_stick, 
    PhilosopherScheduler* scheduler) :
    scheduler(scheduler),
    id(id),
    right_stick(right_stick),
    left_stick(left_stick) {
    eat_signal = make_unique<MySemaphore>(0);
}

Philosopher::~Philosopher() {
}

void Philosopher::run(bool* run_flag) {
  while(*run_flag) {
    // Thinking state.
    // Think for a random amount of time.
    int think_time = rand()%1000;
    this_thread::sleep_for(chrono::milliseconds(think_time));

    // Hungry state.
    cout<<"Phil "<<id<<": Hungry \n";
    // Request required chopsticks from scheduler.
    scheduler->request_sticks(this, eat_signal);
    // Wait for the eat signal.
    eat_signal->Wait();

    // Eating state.
    // Eat for a random amount of time.
    int eat_time = rand()%1000;
    this_thread::sleep_for(chrono::milliseconds(eat_time));
    // Relese the chopsticks.
    scheduler->release_sticks(this);
  }
}
