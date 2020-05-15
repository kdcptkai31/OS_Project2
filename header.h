#pragma once

#include "process.h"
#include "queue.h"
#include "memory.h"
#include <string>

// store a list of processes based on the provided file name
void readData();

// adds any newly arrived procs to the input queue
void add_process(int current_time);

// returns a string (T = x) or a tab (\t) based on whether anything's been
// announced this clock tick
string time_cast(int current_time);

// assigns any available memory to waiting procs that'll fit in there
void extra_memory_helper(int current_time);

// removes any completed procs from memory
void remove_process(int current_time);

// prints the average turnaround time
void print_turnaround_times();

