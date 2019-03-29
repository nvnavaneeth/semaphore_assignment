#include<iostream>
#include<thread>
#include<stdlib.h>
#include<chrono>
#include<ctime>

using namespace std;


const int BUFFER_SIZE = 2;


void display_int_array(int *arr, int length) {
  for (int i = 0; i< length; ++i) 
    cout<<arr[i]<<' ';

  cout<<endl;
}


void producer(int* counter, int* buffer, int n_loops = 100) {
  int in = 0;

  for (int i = 0; i<n_loops; ++i) {
    // Wait some time(Producing the data).
    // int wait_time_ms = rand()%1000 + 1 ;
    // this_thread::sleep_for(chrono::milliseconds(wait_time_ms));

    // Generate some data.
    int new_data = i+5;

    // If the buffer is full dont do anything.
    while(*counter == BUFFER_SIZE);

    // Add newly produced data to the buffer.
    buffer[in] = new_data;
    cout<<"Producer: Adding "<<new_data<<endl;
    // Update variables.
    in = (in + 1)% BUFFER_SIZE;
    *counter += 1;

  }
}

void consumer(int* counter, int* buffer, int n_loops = 100) {
  int out = 0;

  for (int i = 0; i<n_loops; ++i) {
    // If the buffer is empty dont do anything.
    while(*counter == 0);

    // Consume data from the buffer. 
    int new_data = buffer[out];
    cout<<"consumer: Consuming "<<new_data<<endl;
    // Update variables.
    out = (out + 1)%BUFFER_SIZE;
    *counter -= 1;

    // Wait some time(Consuming the data).
    // int wait_time_ms = rand()%1000 + 1;
    // this_thread::sleep_for(chrono::milliseconds(wait_time_ms));
  }
}

int main() {
  int counter = 0;
  int* buffer = new int[BUFFER_SIZE];

  srand(time(0));

  thread producer_thread(producer, &counter, buffer, 5);
  thread consumer_thread(consumer, &counter, buffer, 5);

  // Wait for threads to finish executing.
  producer_thread.join();
  consumer_thread.join();

  delete[] buffer;

  return 0;
}
