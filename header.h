#pragma once

#include "memory.h"
#include <string>

void inputAndInit();

void mainLoop();

void add_process(long current_time);

void extra_memory_helper(long current_time, frameList& frame_list);

void remove_process(long current_time, frameList& frame_list);

void print_turnaround_times();

void  printInputQue();
