#pragma once

//LIBRARIES
#include "memory.h"
#include <string>
#include <fstream>
#include <queue>

//NAMESPACES
using namespace std;

//GLOBALS
const int TIME_MAX = 100000;
const int MEMORY_MAX = 30000;

int processCount = 0;
int pageSize = 0;
int memorySize = 0;

string file_name;
long last_announcement = -1;

vector<process> processList;
deque<process> waitQueue;
frameList* frame_list;

//PROTOTYPES
void inputAndInit();

void mainLoop();

void add_process(long current_time);

void extra_memory_helper(long current_time, frameList& frame_list);

void remove_process(long current_time, frameList& frame_list);

void print_turnaround_times();

void  printInputQue();
