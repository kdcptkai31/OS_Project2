#pragma once
#include "process.h"
#include "queue.h"
#include "memory.h"
#include <string>

// get user input
void get_input();
// store a list of processes based on the provided file name
void readData();

// adds any newly arrived procs to the input queue
void add_new_proc_to_waitList(int current_time);

// returns a string (T = x) or a tab (\t) based on whether anything's been
// announced this clock tick
string correct_format_for_time(int current_time);

// the main program loop
void clock_loop();

// the main function
int main();

// assigns any available memory to waiting procs that'll fit in there
void assign_memory_for_process(int current_time);

// removes any completed procs from memory
void remove_complete_process(int current_time);

// prints the average turnaround time
void print_turnaround_times();

