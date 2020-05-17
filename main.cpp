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
long last_announcement = -1;

vector<process> processList;
deque<process> waitQueue;

int main() {
    
    //Prompts the user for them to enter the memmory size and the page size.
    while ( true ) {
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
    cout << "Please enter the file name: ";
    cin >> file_name;
    
    ifstream myFile;
    myFile.open(file_name);
    
    
    if(!myFile){
        perror("file failed to open");
        exit(-1);
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
    frameList frame_list(memorySize / pageSize, pageSize);
    
    long current_time = 0;
    
    while ( true ) {
        add_process(current_time);
        remove_process(current_time, frame_list);
        extra_memory_helper(current_time, frame_list);
        
        current_time++;
        
        if (current_time > TIME_MAX) {
            cout << "DEADLOCK: max time reached\n";
            break;
        }
        if (waitQueue.size() == 0 && frame_list.isEmpty())
            break;
    }
    print_turnaround_times();
    
    return 0;
}


void add_process(long current_time) {
    process process_;
    string print_time;
    
    for (int i = 0; i < processCount; i++) {
        process_ = processList[i];
        if (process_.start_time == current_time) {
            if (last_announcement == current_time)
                print_time = "\t";
            else
                print_time = "t = " + to_string(current_time) + "\t";
            
            last_announcement = current_time;
            
            cout << print_time << "Process " << process_.pid << " arrives\n";
            
            waitQueue.push_back(process_);
            cout << "Input Queue [ ";
                   for( process e : waitQueue ) {
                       cout << e.pid << " ";
                   }
                   cout << "]\n";
        }
    }
}


void remove_process(long current_time, frameList& frame_list) {
    int i;
    long time_in_memory;
    string result;
    
    for (i = 0; i < processCount; i++) {
        
        time_in_memory = current_time - long(processList[i].time_added_to_memory);
        if (processList[i].is_active && time_in_memory >= processList[i].burst_time) {
            if (last_announcement == current_time)
                result = "\t";
            else
                result = "t = " + to_string(current_time) + "\t";
            
            last_announcement = current_time;
            
            cout << result  << "Process " << processList[i].pid << " completes\n";
            processList[i].is_active = 0;
            processList[i].finish_time = int(current_time);
            frame_list.releaseFrame(processList[i].pid);
            frame_list.printFrames();
            
        }
    }
}


void print_turnaround_times() {
    float total = 0;
    
    for( process e : processList ) {
        total += e.finish_time - e.start_time;
    }
    cout << "Average Turnaround Time: " << total / processCount << endl;
}

void extra_memory_helper(long current_time, frameList& frame_list) {
    process process_;
    string result;
    int limit;
    
    limit = int(waitQueue.size());
    
    for (int i = 0; i < limit; i++ ) {
        process_ = waitQueue.at(i);
        
        if (frame_list.memoryAvailable(process_)) {
            if (last_announcement == current_time)
                result = "\t";
            else
                result = "t = " + to_string(current_time) + "\t";
            
            last_announcement = current_time;
            
            cout << result << "MM moves Process " << process_.pid << " to memory\n";
            
            frame_list.insertProcess(process_);
            
            for (int j = 0; j < processCount; j++) {
                
                if (processList[j].pid == process_.pid) {
                    
                    processList[j].is_active = 1;
                    processList[j].time_added_to_memory = int(current_time);
                    waitQueue.pop_front();
                    limit--;
                }
            }
            cout << "Input Queue [ ";
            for( process e : waitQueue ) {
                cout << e.pid << " ";
            }
            cout << "]\n";
            frame_list.printFrames();
        }
    }
}

