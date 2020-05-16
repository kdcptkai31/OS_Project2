#include "header.h"
#include <fstream>
#include <queue>
 
using namespace std;

const int TIME_MAX = 100000;
const int MEMORY_MAX = 30000;

int processCount = 0;
int pageSize = 0;
int memorySize = 0;

string file_name;
int last_announcement = -1;

vector<process> processList;
process_queue waitList;
frame_list frameList;

int main() {

  while (true) {
    cout << "Please enter memory size(0-30000): ";
    cin >> memorySize;
    cout << "Please enter page size: ";
    cin >> pageSize;

    if (memorySize > 0 && pageSize > 0 && 
        memorySize % pageSize == 0 && 
        memorySize <= MEMORY_MAX)
      break;

    cout << "***ERROR*** Invalid input! Please try again\n";
  }


  readData();


  waitList = create_queue(processCount);


  frameList = create_frame_list(memorySize / pageSize, pageSize);

  long current_time = 0;

  while (true) {

    add_process(current_time);


    remove_process(current_time);


    extra_memory_helper(current_time);

    current_time++;

    if (current_time > TIME_MAX) {

      printf("DEADLOCK: max time reached\n");
      break;
    }

    if (waitList.size == 0 && is_empty_framelist(frameList)) break;

  }

  print_turnaround_times();

  return 0;
}


void readData() {
  cout << "Please enter the file name: ";
  cin >> file_name;

  ifstream myFile;
  myFile.open(file_name);


  if(!myFile){
    perror("file failed to open");
    exit(-1)
  }

  if (myFile.is_open()) {
    myFile >> processCount;

    processList.resize(processCount);

    for (int i = 0; i < processCount; i++) {
      myFile >> processList[i].pid;
      myFile >> processList[i].start_time >> processList[i].burst_time;
      
      int memory_size = 0;
      int memory_sizes[10000] = { 0 };
      int sum = 0;

      myFile >> memory_size;

      for (int j = 0; j < memory_size; j++) {
    
	      myFile >> memory_sizes[j];
        sum += memory_sizes[j];
      }
      
      processList[i].request_memory_size = sum;
      processList[i].is_active = 0;
      processList[i].time_added_to_memory = -1;
      processList[i].finish_time = -1;
    }
  }

  myFile.close();
}


void add_process(int current_time) {
  process process;
  string print_time;

  for (int i = 0; i < processCount; i++) {
    process = processList[i];
    if (process.start_time == current_time) {


      if (last_announcement == current_time)
        print_time = "\t";
      else
        print_time = "t = " + to_string(current_time) + "\t";

      last_announcement = current_time;
  
      cout << print_time << "Process " << process.pid << " arrives\n";

      waitList = enqueue(waitList, process);
      print_queue(waitList);

    }
  }
}



void remove_process(int current_time) {
        
  int i, time_in_memory;
  string result;

  for (i = 0; i < processCount; i++) {
  
    time_in_memory = current_time - processList[i].time_added_to_memory;
    if (processList[i].is_active &&
	time_in_memory >= processList[i].burst_time) {
      if (last_announcement == current_time)
        result = "\t";
      else
        result = "t = " + to_string(current_time) + "\t";

      last_announcement = current_time;

      cout << result  << "Process " << processList[i].pid << " completes\n";
      processList[i].is_active = 0;
      processList[i].finish_time = current_time;
      frameList = free_frame(frameList, processList[i].pid);
      print_list(frameList);
    
    }
  }
}


void print_turnaround_times() {
  int i;
  float total = 0;

  for (i = 0; i < processCount; i += 1)
    total += processList[i].finish_time - processList[i].start_time;

  cout << "Average Turnaround Time: " << total / processCount << endl;
}

void extra_memory_helper(int current_time) {
  process process;
  string result;
  int index, limit;

  limit = waitList.size;


  for (int i = 0; i < limit; i++) {
  
    index = get_index(waitList, i);
    process = waitList.elements[index];

    if (check_available_memory(frameList, process)) {
      if (last_announcement == current_time)
        result = "\t";
      else
        result = "t = " + to_string(current_time) + "\t";

      last_announcement = current_time;

      cout << result << "MM moves Process " << process.pid << " to memory\n";

      frameList = enqueue_process(frameList, process);
      
        for (int j = 0; j < processCount; j++) {

          if (processList[j].pid == process.pid) {
            
	          processList[j].is_active = 1;
            processList[j].time_added_to_memory = current_time;
            waitList = dequeue_process(waitList, i);

          }
        }

        print_queue(waitList);
        print_list(frameList);
        
    }
  }
}

