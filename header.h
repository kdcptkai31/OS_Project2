#pragma once

#include "memory.h"
#include <string>

void add_process(long current_time);

void extra_memory_helper(long current_time, frameList frame_list);

void remove_process(long current_time, frameList frame_list);

void print_turnaround_times();

void assign_memory_for_process(long crrent_time, frameList frame_list);

