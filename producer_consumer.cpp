#include<iostream>
#include<thread>
#include<stdlib.h>
#include<chrono>
#include<ctime>
#include "my_semaphore.h"
#include "my_mutex.h"

using namespace std;


// Defines the size of the buffer.
const int BUFFER_SIZE = 3;
// Position in  buffer to which data is to be next written.
int write_pos = 0;
// Position in buffer from which data is to be next read.
int read_pos = 0;

// Semaphore that stores the amount of buffer space available to write to.
MySemaphore sem_full(BUFFER_SIZE);
// Semaphore that stores the count of data yet ot be read from buffer.
MySemaphore sem_empty(0);
// Mutex to protect the buffer, write_pos and read_pos in case of multiple
// producers and consumers.
MyMutex mtx;


void producer(int* counter, int* buffer, int producer_id, int n_loops = 100) {
  int in = 0;

  for (int i = 0; i<n_loops; ++i) {
    // Generate some data.
    int new_data = i+1;
    // If the buffer is full dont do anything.
    sem_full.Wait();
    mtx.Acquire();

    // Add newly produced data to the buffer.
    buffer[write_pos] = new_data;
    cout<<"Producer "<<producer_id<<": Adding to "<<write_pos<<endl;
    write_pos = (write_pos + 1)% BUFFER_SIZE;

    mtx.Release();
    sem_empty.Signal();

  }
}


void consumer(int* counter, int* buffer, int consumer_id, int n_loops = 100) {
  int out = 0;

  for (int i = 0; i<n_loops; ++i) {
    // If the buffer is empty dont do anything.
    sem_empty.Wait();
    mtx.Acquire();

    // Consume data from the buffer. 
    int new_data = buffer[read_pos];
    cout<<"Consumer "<<consumer_id<<": Reading from "<<read_pos<<endl;
    read_pos = (read_pos + 1)%BUFFER_SIZE;

    mtx.Release();
    sem_full.Signal();
  }
}


int main() {
  int counter = 0;
  int* buffer = new int[BUFFER_SIZE];

  srand(time(0));

  thread producer_thread1(producer, &counter, buffer, 1, 5);
  thread producer_thread2(producer, &counter, buffer, 2, 5);
  // Start consumer threads after some time to see that producer thread is 
  // blocking once the buffer is full.
  this_thread::sleep_for(chrono::milliseconds(3000));
  thread consumer_thread1(consumer, &counter, buffer, 1, 5);
  thread consumer_thread2(consumer, &counter, buffer, 2, 5);

  // Wait for threads to finish executing.
  producer_thread1.join();
  producer_thread2.join();
  consumer_thread1.join();
  consumer_thread2.join();

  delete[] buffer;

  return 0;
}
