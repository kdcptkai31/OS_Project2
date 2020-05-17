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
    cout << "release\n";
    for ( frame e : frames) {
        if(e.assignedID == pid ) {
            e.assignedID = 0;
            e.pageNum = 0;
            e.assigned = false;
        }
    }
}

bool frameList::isEmpty() {
    cout << "isempty\n";
    for ( frame e : frames ) {
        if( e.assigned )
            return false;
    }
    return true;
}


int frameList::memoryAvailable(process proc) {
   // cout << "memory av\n";
    int free = 0;
    for( int i = 0; i < records; i++) {
        if( !frames.at(i).assigned )
            free++;
    }
    return ( free * pageSize ) >= proc.request_memory_size;
}


void frameList::insertProcess(process proc) {
    cout << "insert\n";
    int memory, page = 1;
    memory = proc.request_memory_size;
    
    for ( frame e : frames ) {
        if(!e.assigned) {
            e.assigned = true;
            e.pageNum = page;
            e.assignedID = proc.pid;
            
            page++;
            memory -= pageSize;
        }
         if (memory <= 0) break;
    }
}


void frameList::printFrames() {
    cout << "print\n";
    bool free = false;
    int begin = 0, i = 0;;
    
    cout << "\tMemory map:\n";
    
    for ( frame e : frames ) {
        i++;
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
    }
    if (free) {
      cout << "\t\t" << begin * pageSize<< "-"
             << ((records) * pageSize) - 1 << ": Free frame(s)\n";
          
    }
}


