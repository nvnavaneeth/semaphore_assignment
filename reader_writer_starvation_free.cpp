// Implementation of readers-writers problem in a starvation free manner. This
// implementation uses a straightforward method to avoid starvation, using a
// sort of FCFS method, selecting readers or a writer task depending on who
// came first.

#include<iostream>
#include<thread>
#include<mutex>
#include<queue>
#include<chrono>
#include<condition_variable>

using namespace std;

const int TYPE_NONE = -1;
const int TYPE_READER = 0;
const int TYPE_WRITER = 1;

queue<condition_variable*> cv_queue;
queue<int> type_queue;

int active_task_type = TYPE_NONE;
int reader_count = 0;

mutex q_mtx;
mutex rw_mtx;


void sched_next_task() {
  rw_mtx.lock();
  if (active_task_type != TYPE_NONE) {
    rw_mtx.unlock();
    return;
  }
  rw_mtx.unlock();

  q_mtx.lock();
  if (cv_queue.empty()) {
    // No task to be scheduled.
    q_mtx.unlock();
    return ;
  }

  if (type_queue.front() == TYPE_WRITER) {
    // Wake up one writer.
    auto* cv = cv_queue.front();
    cv->notify_one();

    cv_queue.pop();
    type_queue.pop();
  }
  else {
    // Wake up all the readers in the begining of the queue.
    while(!cv_queue.empty() && type_queue.front() == TYPE_READER) {
      auto* cv = cv_queue.front();
      cv->notify_one();
      cv_queue.pop();
      type_queue.pop();
    }
  } 

  q_mtx.unlock();
}


void reader(int reader_id) {
  unique_lock<mutex> lck(q_mtx);
  rw_mtx.lock();
  // If there are other tasks in queue, or a writer is active, wait in queue.
  if (!cv_queue.empty() || active_task_type == TYPE_WRITER) {
    rw_mtx.unlock();
    condition_variable* cv = new condition_variable();
    cv_queue.push(cv);
    type_queue.push(TYPE_READER);

    cv->wait(lck);
    rw_mtx.lock();
  }
  lck.unlock();

  active_task_type = TYPE_READER; reader_count++;
  rw_mtx.unlock();

  // Reading in progress....
  cout<<"Reader "<<reader_id<<": Reading\n";
  this_thread::sleep_for(chrono::milliseconds(2000));
  cout<<"Reader "<<reader_id<<": Finished reading\n";

  rw_mtx.lock();
  reader_count--;
  if (reader_count == 0)
    active_task_type = TYPE_NONE;
  rw_mtx.unlock();

  // Wakeup next task from queue. 
  sched_next_task();
}


void writer(int writer_id) {
  unique_lock<mutex> lck(q_mtx);
  rw_mtx.lock();
  // If there are other tasks in queue, or a reader/writer is active, wait in queue.
  if (!cv_queue.empty() || active_task_type != TYPE_NONE) {
    rw_mtx.unlock();

    condition_variable* cv = new condition_variable();
    cv_queue.push(cv);
    type_queue.push(TYPE_WRITER);
    cv->wait(lck);
    rw_mtx.lock();
  }
  lck.unlock();

  active_task_type = TYPE_WRITER;
  rw_mtx.unlock();

  // Writing in progress....
  cout<<"Writer "<<writer_id<<": Writing\n";
  this_thread::sleep_for(chrono::milliseconds(3000));
  cout<<"Writer "<<writer_id<<": Finished writing\n";

  rw_mtx.lock();
  active_task_type = TYPE_NONE;
  rw_mtx.unlock();

  // Wakeup next task from queue. 
  sched_next_task();
}


int main() {
  thread reader_thread1(reader, 1);
  thread reader_thread2(reader, 2);
  thread writer_thread1(writer, 1);
  thread writer_thread2(writer, 2);
  thread reader_thread3(reader, 3);

  reader_thread1.join();
  reader_thread2.join();
  reader_thread3.join();
  writer_thread1.join();
  writer_thread2.join();

  return 0;
}
