#pragma once

#include "process.h"
#include "queue.h"
#include "memory.h"
#include <string>

void readData();

void add_process(int current_time);

string time_cast(int current_time);

void extra_memory_helper(int current_time);

void remove_process(int current_time);

void print_turnaround_times();

void assign_memory_for_process(int crrent_time);
