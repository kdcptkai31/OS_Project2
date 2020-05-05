#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include "header.h"
using namespace std;

const int TIME_MAX = 100000;
const int MEMORY_SIZE_MAX = 30000;

int number_of_processes = 0;
int page_size = 0;
int memory_size = 0;

string file_name = "";
int last_announcement = -1;

vector<PROCESS> processList;
proc_queue waitList;
frame_list frameList;

int main() {

        // get the user input
        get_input();

        // read values from the input'd file into a shared proc list
        readData();

        // create a shared queue with a capacity = # of procs
        waitList = create_wait_queue(number_of_processes);

        // create a shared frameList
        frameList = create_frame_list(memory_size / page_size, page_size);

        clock_loop();

        //system("pause");
        return 0;
}

// the main program loop
void clock_loop() {
        long current_time = 0;

        while (1) {
                // queue any procs that have arrived
                add_new_proc_to_waitList(current_time);

                // remove any completed procs
                remove_complete_process(current_time);

                // assign available memory to procs that need it
                assign_memory_for_process(current_time);

                current_time++;

                if (current_time > TIME_MAX) {
                        printf("DEADLOCK: max time reached\n");
                        break;
                }

                if (waitList.size == 0 && frame_list_is_empty(frameList)) {
                        break;
                }
        }

        print_turnaround_times();
}

// get user input
void get_input() {
        while (true) {
                cout << "Please enter memory size(0-30000): ";
                cin >> memory_size;
                cout << "Please enter page size: ";
                cin >> page_size;

                if (memory_size > 0 && page_size > 0 && (memory_size) % (page_size) == 0 && memory_size <= MEMORY_SIZE_MAX)
                        break;

                cout << "ERROR!!! "
                        << "Invalid input!!! "
                        << "Please enter again."
                        << endl;
        }
}

// creates a list of processes based on the provided filePath
void readData() {
        cout << "Please enter the file name: ";
        cin >> file_name;

        ifstream myFile;
        myFile.open(file_name);

        if(!myFile){
                perror("file failed to open");
                readData();
        }
        if (myFile.is_open()) {
                //get number of processes
                myFile >> number_of_processes;
                if(myFile.fail()){
                        perror("input file error number of processes");
                        exit(-1);
                }
                processList.resize(number_of_processes);

                for (int i = 0; i < number_of_processes; i++) {
                        //set id
                        myFile >> processList[i].pid;
                        if(myFile.fail()){
                                perror("input file error id number");
                                exit(-1);
                        }
                        //set arriving time and life time
                        myFile >> processList[i].arriving_time >> processList[i].burst_time;
                        if(myFile.fail()){
                                perror("input file error arriving time/ life time");
                                exit(-1);
                        }
                        //set memory size
                        int memory_request_num = 0;
                        int memory_request_size[10000] = { 0 };
                        int sum = 0;
                        myFile >> memory_request_num;
                        if(myFile.fail()){
                                perror("input file error memory size");
                                exit(-1);
                        }
                        for (int j = 0; j < memory_request_num; j++) {
                                myFile >> memory_request_size[j];
                                sum += memory_request_size[j];
                        }
                        processList[i].request_memory_size = sum;

                        //initialize other data in processList
                        processList[i].is_active = 0;
                        processList[i].time_added_to_memory = -1;
                        processList[i].time_finished = -1;
                }
        }
        //close file
        myFile.close();
}

// adds any newly arrived procs to the input queue
void add_new_proc_to_waitList(int current_time) {
        PROCESS proc;
        for (int i = 0; i < number_of_processes; i++) {
                proc = processList[i];
                if (proc.arriving_time == current_time) {
                        string print_time = correct_format_for_time(current_time);
                        cout << print_time << "Process "
                                << proc.pid << " arrives" << endl;

                        waitList = add_process_to_queue(waitList, proc);
                        print_proc_queue(waitList);
                }
        }
}

// returns a string (T = x) or a tab (\t) based on whether anything's been
// announced this clock tick
string correct_format_for_time(int current_time) {
        string result = "";

        if (last_announcement == current_time)
                result = "\t";
        else
                result = "t = " + to_string(current_time) + "\t";

        last_announcement = current_time;
        return result;
}

// removes any completed procs from memory
void remove_complete_process(int current_time) {
        int i, time_spent_in_memory;
        // dequeue any procs that need it
        for (i = 0; i < number_of_processes; i++) {
                time_spent_in_memory = current_time - processList[i].time_added_to_memory;
                if (processList[i].is_active && (time_spent_in_memory >= processList[i].burst_time)) {
                        cout << correct_format_for_time(current_time)  << "Process " << processList[i].pid << " completes" << endl;
                        processList[i].is_active = 0;
                        processList[i].time_finished = current_time;
                        frameList = free_memory_for_pid(frameList, processList[i].pid);
                        print_frame_list(frameList);
                }
        }
}

// prints the average turnaround time
void print_turnaround_times() {
        int i;
        float total = 0;
        for (i = 0; i < number_of_processes; i += 1) {
                total += processList[i].time_finished - processList[i].arriving_time;
        }
        cout << "Average Turnaround Time " << total / number_of_processes <<endl;
}

// assigns any available memory to waiting procs that'll fit in there
void assign_memory_for_process(int current_time) {
        int index, limit;
        PROCESS proc;

        limit = waitList.size;

        // enqueue any procs that can be put into mem
        for (int i = 0; i < limit; i++) {
                index = iterate_queue_index(waitList, i);
                proc = waitList.elements[index];

                if (valid_memory_check(frameList, proc)) {
                        cout << correct_format_for_time(current_time) << "MM moves Process "
                                << proc.pid << " to memory" << endl;

                        frameList = add_process_to_queue_into_memory(frameList, proc);
                        for (int j = 0; j < number_of_processes; j++) {
                                if (processList[j].pid == proc.pid) {
                                        processList[j].is_active = 1;
                                        processList[j].time_added_to_memory = current_time;
                                        waitList = delete_process_for_queue(waitList, i);
                                }
                        }

                        print_proc_queue(waitList);
                        print_frame_list(frameList);
                }
        }
}

