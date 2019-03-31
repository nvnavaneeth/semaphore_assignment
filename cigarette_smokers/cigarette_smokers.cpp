#include<chrono>
#include<ctime>
#include<iostream>
#include<memory>
#include<thread>
#include<vector>
#include"../my_semaphore.h"

using namespace std;

const int SMOKER_TOBACCO = 0;
const int SMOKER_PAPER = 1;
const int SMOKER_MATCH = 2;

// bool array that shows whether thread corresponding to a smoker is running.
bool running[3] = {0};

// Semaphores used to let smokers know that they can smoke.
vector<unique_ptr<MySemaphore>> smoke_signal;
// Semaphore used to signal that a smoker has finished smoking.
MySemaphore done_signal(0);

// Flag to control when to stop all threads.
bool agent_run_flag = true;
bool smokers_run_flag = true;

void agent() {
  while(agent_run_flag) {
    int rand_n = rand()%3; 
    switch(rand_n) {
      case 0: // Put paper and match on the table and let the smoker with 
              // tobacco know that he can smoke.
              if(running[SMOKER_TOBACCO]) {
                smoke_signal[SMOKER_TOBACCO]->Signal();
                // Wait for smoker to complete smoking.
                done_signal.Wait();
              }
              break;

      case 1: // Put match and tobacco on the table and let the smoker with 
              // paper know that he can smoke.
              if(running[SMOKER_PAPER]) {
                smoke_signal[SMOKER_PAPER]->Signal();
                // Wait for smoker to complete smoking.
                done_signal.Wait();
              }
              break;

      case 2: // Put tobacco and paper on the table and let the smoker with 
              // match know that he can smoke.
              if(running[SMOKER_MATCH]){
                smoke_signal[SMOKER_MATCH]->Signal();
                // Wait for smoker to complete smoking.
                done_signal.Wait();
              }
              break;
    }
  }
}


void smoker_tobacco() {
  running[SMOKER_TOBACCO] = true;
  while(smokers_run_flag) {
    // Wait for signal from agent that other two resourcer have been put on
    // the table.
    cout<<"Smoker tobacco: Waiting\n";
    smoke_signal[SMOKER_TOBACCO]->Wait();

    // Smoke for random amount of time.
    cout<<"Smoker tobacco: Smoking\n";
    int smoke_time = rand()%2000;
    this_thread::sleep_for(chrono::milliseconds(smoke_time));

    // Let agent know that he can put next items on the table.
    cout<<"Smoker tobacco: Done smoking\n";
    done_signal.Signal();
  }
  cout<<"Smoker tobacco: exiting\n";
  running[SMOKER_TOBACCO] = false;
}

void smoker_paper() {
  running[SMOKER_PAPER] = true;
  while(smokers_run_flag) {
    // Wait for signal from agent that other two resourcer have been put on
    // the table.
    cout<<"Smoker paper: Waiting\n";
    smoke_signal[SMOKER_PAPER]->Wait();

    // Smoke for random amount of time.
    cout<<"Smoker paper: Smoking\n";
    int smoke_time = rand()%2000;
    this_thread::sleep_for(chrono::milliseconds(smoke_time));

    // Let agent know that he can put next items on the table.
    cout<<"Smoker paper: Done smoking\n";
    done_signal.Signal();
  }
  cout<<"Smoker paper: exiting\n";
  running[SMOKER_PAPER] = false;
}

void smoker_match() {
  running[SMOKER_MATCH] = true;
  while(smokers_run_flag) {
    // Wait for signal from agent that other two resourcer have been put on
    // the table.
    cout<<"Smoker match: Waiting\n";
    smoke_signal[SMOKER_MATCH]->Wait();

    // Smoke for random amount of time.
    cout<<"Smoker match: Smoking\n";
    int smoke_time = rand()%2000;
    this_thread::sleep_for(chrono::milliseconds(smoke_time));

    // Let agent know that he can put next items on the table.
    cout<<"Smoker match: Done smoking\n";
    done_signal.Signal();
  }
  cout<<"Smoker match: exiting\n";
  running[SMOKER_MATCH] = false;
}

int main() {

  // Initialzie smoke_signal vector.
  for (int i = 0;i < 3; ++i) 
    smoke_signal.push_back(make_unique<MySemaphore>(0));

  thread th_smoker_tobacco(smoker_tobacco);
  thread th_smoker_paper(smoker_paper);
  thread th_smoker_match(smoker_match);
  thread th_agent(agent);

  // Let the program run for sometime.
  this_thread::sleep_for(chrono::seconds(6));

  // Stop all threads.
  smokers_run_flag = false;
  th_smoker_tobacco.join();
  th_smoker_paper.join();
  th_smoker_match.join();

  agent_run_flag = false;
  th_agent.join();

  return 0;
}
