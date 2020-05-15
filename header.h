#pragma once

#include "queue.h"
#include "memory.h"
#include <string>

class process{
public:
  int pid;
  int start_time;
  int burst_time;
  int request_memory_size;
  int time_added_to_memory;
  int is_active;
  int finish_time;
};

void readData();

void add_process(int current_time);

string time_cast(int current_time);

void extra_memory_helper(int current_time);

void remove_process(int current_time);

void print_turnaround_times();

void assign_memory_for_process(int crrent_time);
