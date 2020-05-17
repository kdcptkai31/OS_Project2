#pragma once

#include "process.h"
#include "memory.h"
#include <string>

void readData();

void add_process(long current_time);

void extra_memory_helper(long current_time);

void remove_process(long current_time);

void print_turnaround_times();

void assign_memory_for_process(long crrent_time);
