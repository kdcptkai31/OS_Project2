#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct proc_queue {

  int capacity;
  int size;
  int front;
  int tail;
  vector<PROCESS> elements;

};

proc_queue create_wait_queue(int length) {
    
  proc_queue q;
  q.elements.resize(length);
  q.size = 0;
  q.capacity = length;
  q.front = 0;
  q.tail = -1;

  return q;

}

proc_queue add_process_to_queue(proc_queue q, PROCESS proc) {

  if (q.size == q.capacity) {
  
    cout << "ERROR: queue is full to capacity!" << endl;
    exit(2);
    
  }

  q.size++;
  q.tail++;

  if (q.tail == q.capacity)
    q.tail = 0;

  q.elements[q.tail] = proc;
  return q;

}

PROCESS peek_queue_at_index(proc_queue q, int index) {return q.elements[index];}

int iterate_queue_index(proc_queue q, int index) {return q.front + index;}

void print_proc_queue(proc_queue q) {

  PROCESS proc;

  cout << "\tInput queue: [ ";
  for (int i = 0; i < q.size; i ++) {
  
    proc = peek_queue_at_index(q, iterate_queue_index(q, i));
    cout << proc.pid << " ";
   
  }
 
  cout << "]" << endl;

}

int has_next_element(proc_queue q) {return q.size == 0 ? 0 : 1;}

void dequeue_proc(proc_queue q) {

  if (!has_next_element(q)) {
    cout << "ERROR: queue is empty, can't dequeue anything." << endl;
    exit(2);
    
  }

  q.size--;
  q.front++;

  if (q.front == q.capacity)
    q.front = 0;

}

proc_queue delete_process_for_queue(proc_queue q, int index) {

  int prev = 0;
  for (int i = 0; i < q.size; i += 1) {
  
    if (i > index)
      q.elements[prev] = q.elements[i];

    prev = i;
    
  }

  q.size--;
  q.tail--;

  return q;

}

