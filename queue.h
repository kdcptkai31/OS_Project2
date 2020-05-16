#pragma once

#include <iostream>
#include <vector>
#include "process.h"

using namespace std;

struct process_queue {
  int capacity;
  int size;
  int head;
  int tail;

  vector<process> elements;
};

process_queue create_queue(int length) {
  process_queue queue;

  queue.elements.resize(length);
  queue.size = 0;
  queue.capacity = length;
  queue.head = 0;
  queue.tail = -1;

  return queue;
}

bool is_full(process_queue queue){ return (queue.size == queue.capacity); }

process_queue enqueue(process_queue queue, process process) {

  if (is_full(queue)) {
  
    cout << "*** Error *** queue is full!" << endl;
    exit(-1);
  }

  queue.size++;
  queue.tail++;

  if (queue.tail == queue.capacity) { queue.tail = 0; }

  queue.elements[queue.tail] = process;
  return queue;

}

process get_process(process_queue q, int index) {return q.elements[index];}

int get_index(process_queue q, int index) {return q.head + index;}

void print_queue(process_queue q) {

  process proc;

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
  q.head++;

  if (q.head == q.capacity)
    q.head = 0;

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

