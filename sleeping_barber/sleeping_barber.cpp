#include<iostream>
#include<thread>
#include<chrono>
#include<ctime>
#include<vector>
#include"../my_semaphore.h"
#include"../my_mutex.h"

using namespace std;

// Number of chairs.
const int N_CHAIRS = 5;

// Semaphore indicating if barber is ready. Initially sleeping.
MySemaphore barber_sem(0);
//Semaphore indicating number of waiting customers.
MySemaphore customers_sem(0);
// Number of waiting customers. We need to keep this variable as we cannot
// obtain the value of the counting variable from the semaphore.
int n_waiting = 0;
// Mutex lock to protect n_waiting.
MyMutex mtx;
// Semaphore to indicate that barber has finished cutting and the customer 
// can leave.
MySemaphore finished_cutting_sem(0);

// Flags to control the termination of the threads.
bool customer_run_flag = true;

void barber() {
  while(true) {
    // Exit condition.
    mtx.Acquire();
    if (!customer_run_flag && n_waiting == 0) {
      mtx.Release();
      return;
    }
    mtx.Release();

    // Wait for a customer. Blocks (sleeps) as long as there are no customers.
    cout<<"Barber : Next customer\n";
    customers_sem.Wait();

    // Ublocks one customer which has been waiting on the barber.
    barber_sem.Signal();

    // Reduce count of waiting customers.
    mtx.Acquire();
    n_waiting--;
    mtx.Release();

    // Cutting hair.
    int cut_time = rand()%5000;
    this_thread::sleep_for(chrono::milliseconds(cut_time));

    // Finished cutting, inform the customer.
    finished_cutting_sem.Signal();
  }
}

void customer(int customer_id) {
  mtx.Acquire();
  if (n_waiting < N_CHAIRS) {
    n_waiting++;
    mtx.Release();
    
    customers_sem.Signal();
    
    // Wait for the barber until this customers turn arrives.
    cout<<"Customer "<<customer_id<<": waiting in chair\n";
    barber_sem.Wait();
    // Barber is cutting the hair. Wait for him to finish.
    cout<<"Customer "<<customer_id<<": cutting hair\n";
    finished_cutting_sem.Wait();
  }
  else{
    // No free chairs, leave.
    cout<<"Customer "<<customer_id<<": No space, leaving\n";
    mtx.Release();
  }
}

void customer_thread_manager() {
  vector<thread> customer_thread_vec;
  int customer_id = 0;
  while(customer_run_flag) {
    customer_thread_vec.push_back(thread(customer, ++customer_id));
    // Wait some time before sending another customer.
    this_thread::sleep_for(chrono::milliseconds(rand()%1000));
  }

  for (auto& customer_thread : customer_thread_vec)
    customer_thread.join();
}

int main() {
  srand(time(0));

  thread barber_thread(barber);
  thread ctm_thread(customer_thread_manager);

  // Run for some time.
  this_thread::sleep_for(chrono::seconds(4));
  cout<<"Exiting after attending to remaining customers.\n";

  customer_run_flag = false;
  ctm_thread.join();
  barber_thread.join();

  return 0;
}
