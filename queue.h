#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct process_queue {

  int capacity;
  int size;
  int front;
  int tail;
  vector<PROCESS> elements;

};

process_queue create_queue(int length) {
    
  process_queue queue;

  queue.elements.resize(length);
  queue.size = 0;
  queue.capacity = length;
  queue.front = 0;
  queue.tail = -1;

  return queue;

}

process_queue enqueue(process_queue queue, PROCESS process) {

  if (queue.size == queue.capacity) {
  
    cout << "ERROR: queue is full!" << endl;
    exit(2);
    
  }

  queue.size++;
  queue.tail++;

  if (queue.tail == queue.capacity) { queue.tail = 0; }

  queue.elements[queue.tail] = process;
  return queue;

}

PROCESS get_process(process_queue q, int index) {return q.elements[index];}

int get_index(process_queue q, int index) {return q.front + index;}

void print_queue(process_queue q) {

  PROCESS proc;

  cout << "\tInput queue: [ ";
  for (int i = 0; i < q.size; i ++) {
  
    proc = get_process(q, get_index(q, i));
    cout << proc.pid << " ";
   
  }
 
  cout << "]" << endl;

}

int has_next(process_queue q) {return q.size == 0 ? 0 : 1;}

void dequeue_proc(process_queue q) {

  if (!has_next(q)) {
    cout << "ERROR: queue is empty, can't dequeue anything." << endl;
    exit(2);
    
  }

  q.size--;
  q.front++;

  if (q.front == q.capacity)
    q.front = 0;

}

process_queue dequeue_process(process_queue q, int index) {

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

