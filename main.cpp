#include "header.h"
#include <fstream>
#include <queue>
 
using namespace std;

const int TIME_MAX = 100000;
const int MEMORY_MAX = 30000;

int processCount = 0;
int pageSize = 0;
int memorySize = 0;

string file_name = "";
int last_announcement = -1;

vector<PROCESS> processList;
process_queue waitList;
frame_list frameList;

int main() {
  // retrieve user input
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

  // read values from file into a shared process list
  readData();

  // create a shared queue with a capacity = # of procs
  waitList = create_queue(processCount);

  // create a shared frameList
  frameList = create_frame_list(memorySize / pageSize, pageSize);

  long current_time = 0;

  while (1) {
    // queue any procs that have arrived
    add_process(current_time);

    // remove any completed procs
    remove_process(current_time);

    // assign available memory to procs that need it
    assign_memory_for_process(current_time);

    current_time++;

    if (current_time > TIME_MAX) {

      printf("DEADLOCK: max time reached\n");
      break;
    }

    if (waitList.size == 0 && is_empty(frameList)) break;

  }//End while loop

  print_turnaround_times();

  return 0;
}

// Creates list of processes from input file
void readData() {
  cout << "Please enter the file name: ";
  cin >> file_name;

  ifstream myFile;
  myFile.open(file_name);

  //Check for proper opening of file
  if(!myFile){
    perror("file failed to open");
    readData();
  }

  if (myFile.is_open()) {
    myFile >> processCount;

    if(myFile.fail()){

      perror("\nInput file error - number of processes");
      exit(-1);

    }

    processList.resize(processCount);

    for (int i = 0; i < processCount; i++) {
      myFile >> processList[i].pid;
      if(myFile.fail()){

        perror("\nInput file error - id number");
        exit(-1);

      }

      myFile >> processList[i].start_time >> processList[i].burst_time;
      
      if(myFile.fail()){

        perror("\nInput file error - time data");
        exit(-1);

      }

      //set memory size
      int memory_size = 0;
      int memory_sizes[10000] = { 0 };
      int sum = 0;

      myFile >> memory_size;

      if(myFile.fail()){

        perror("\nInput file error - memory size");
        exit(-1);

      }

      for (int j = 0; j < memory_size; j++) {
        
	      myFile >> memory_sizes[j];
        sum += memory_sizes[j];

      }
      
      processList[i].request_memory_size = sum;

      //initialize other data in processList
      processList[i].is_active = 0;
      processList[i].time_added_to_memory = -1;
      processList[i].finish_time = -1;

    }
  }
  
  //close file
  myFile.close();

}

// adds any newly arrived procs to the input queue
void add_process(int current_time) {
  
  PROCESS process;
  for (int i = 0; i < processCount; i++) {

    process = processList[i];
    if (process.start_time == current_time) {

      string print_time = time_cast(current_time);
      cout << print_time << "Process " << process.pid << " arrives\n";

      waitList = enqueue(waitList, process);
      print_queue(waitList);

    }
  }
}

// returns a string (T = x) or a tab (\t) based on new processes
string time_cast(int current_time) {
       
  string result = "";

  if (last_announcement == current_time)
    result = "\t";
  else
    result = "t = " + to_string(current_time) + "\t";

  last_announcement = current_time;
  return result;

}

// removes any completed processes from memory
void remove_process(int current_time) {
        
  int i, time_in_memory;
  // dequeue any procs that need it
  for (i = 0; i < processCount; i++) {
  
    time_in_memory = current_time - processList[i].time_added_to_memory;
    if (processList[i].is_active &&
	time_in_memory >= processList[i].burst_time) {

      cout << time_cast(current_time)  << "Process " << processList[i].pid << " completes\n";
      processList[i].is_active = 0;
      processList[i].finish_time = current_time;
      frameList = free_frame(frameList, processList[i].pid);
      print_list(frameList);
    
    }
  }
}

// prints the average turnaround time
void print_turnaround_times() {
  int i;
  float total = 0;

  for (i = 0; i < processCount; i += 1)
    total += processList[i].finish_time - processList[i].start_time;

  cout << "Average Turnaround Time: " << total / processCount << endl;
}

// Assigns any available memory to waiting processes
void assign_memory_for_process(int current_time) {
  PROCESS process;
  int index, limit;

  limit = waitList.size;

  // Enqueue any processes that can fit in available memory
  for (int i = 0; i < limit; i++) {
  
    index = get_index(waitList, i);
    process = waitList.elements[index];

    if (check_available_memory(frameList, process)) {
    
      cout << time_cast(current_time) << "MM moves Process " << process.pid << " to memory\n";

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

