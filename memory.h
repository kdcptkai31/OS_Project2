#pragma once
#include <vector>
#include <iostream>
#include "header.h"

class process{
public:
  int pid;
  int start_time;
  int burst_time;
  int request_memory_size;
  int time_added_to_memory;
  int is_active;
  int finish_time;
};

using namespace std;
struct frame {
  bool assigned;
  int assignedID;
  int pageNum;
};

class frameList {
public:
    frameList();
    frameList(int entries, int pageSize);
    void releaseFrame(int pid);
    bool isEmpty();
    int memoryAvailable(process proc);
    
    void insertProcess(process proc);
    void printFrames();
    void print();
    
    vector<frame> frames;
    int records;
    int pageSize;
};


frameList::frameList() {}

frameList::frameList(int recordsIn, int pageSizeIn) {
    frames.resize(recordsIn);
    pageSize = pageSizeIn;
    records = recordsIn;
    
    for( frame e : frames ) {
        e.pageNum = 0;
        e.assignedID = 0;
        e.assigned = false;
    }
}

void frameList::releaseFrame(int pid) {
    for ( frame e : frames) {
        if(e.assignedID == pid ) {
            e.assignedID = 0;
            e.pageNum = 0;
            e.assigned = false;
        }
    }
}

bool frameList::isEmpty() {
    for ( frame e : frames ) {
        if( e.assigned )
            return false;
    }
    return true;
}


int frameList::memoryAvailable(process proc) {
    int free = 0;
    for( int i = 0; i < records; i++) {
        if( !frames.at(i).assigned )
            free++;
    }
    return ( free * pageSize ) >= proc.request_memory_size;
}


void frameList::insertProcess(process proc) {
    int memory, page = 1;
    memory = proc.request_memory_size;
    cout << "Mem size: " << proc.request_memory_size << endl;
    for ( int i = 0; i < records; i++ ) {
        if(!frames.at(i).assigned) {
            frames.at(i).assigned = true;
            frames.at(i).pageNum = page;
            frames.at(i).assignedID = proc.pid;
            
            
            page++;
            memory -= pageSize;
        }
        if (memory <= 0) 
            break;
    }
}


void frameList::printFrames() {
    bool free = false;
    int begin = 0, i = 0;;
    
    cout << "\tMemory map:\n";
    
    for ( frame e : frames ) {
        if( !free && !e.assigned ) {
            free = true;
            begin = i;
        } else if ( free && e.assigned ) {
            free = false;
            cout << "\t\t" << begin * pageSize << "-" << (i * pageSize) - 1 << ": Free frame(s)\n";
        }
        
        if( e.assigned ) {
            cout << "\t\t" << i * pageSize << "-" << ((i + 1) * pageSize) - 1 << ": Process" << e.assignedID << ", Page " << e.pageNum << endl;
        }
        i++;
    }
    if (free) {
      cout << "\t\t" << begin * pageSize<< "-" << ((records) * pageSize) - 1 << ": Free frame(s)\n";
          
    }
}


void frameList::print() {
    cout << "Printing all frame info: \n";
    for ( frame e : frames ) {
        cout << "\t" << e.assignedID << " " << e.pageNum << " assigned: ";
        if(e.assigned)
            cout << "yes\n";
        else
            cout << "no\n";
    }
}
