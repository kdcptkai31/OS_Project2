#pragma once
#include <vector>
#include "process.h"

using namespace std;

struct frame {
  bool assigned;
  int assigned_id;
  int pageNum;
};

struct frame_list {
  vector<frame> frames;
  int entries;
  int page_size;
};

frame_list create_frame_list(int entries, int pageSize) {
  frame_list frameList;
  frameList.frames.resize(entries);
  frameList.page_size = pageSize;
  frameList.entries = entries;

  for (int i = 0; i < frameList.entries; i ++) {
    frameList.frames[i].pageNum = 0;
    frameList.frames[i].assigned_id = 0;
    frameList.frames[i].assigned = false;
  }

  return frameList;

}


frame_list free_frame(frame_list list, int pid) {
  for (int i = 0; i < list.entries; i++) {
    if (list.frames[i].assigned_id == pid) {
      list.frames[i].assigned_id = 0;
      list.frames[i].pageNum = 0;
      list.frames[i].assigned = false;
    }
  }

  return list;
}

bool is_empty_framelist(frame_list list) {
        
  for (int i = 0; i < list.entries; i++){ if (list.frames[i].assigned) return false; }
    
  return true;
}

int check_available_memory(frame_list list, process proc) {
  
  int num_free_frames = 0;

  for (int i = 0; i < list.entries; i++){ if (!list.frames[i].assigned) num_free_frames++; }
        

  return (num_free_frames * list.page_size) >= proc.request_memory_size;

}

frame_list enqueue_process(frame_list list, process proc) {


  int remaining_mem, current_page = 1;

  remaining_mem = proc.request_memory_size;

  for (int i = 0; i < list.entries; i++) {
  
    if (!list.frames[i].assigned) {
                        
      list.frames[i].assigned = true;
      list.frames[i].pageNum = current_page;
      list.frames[i].assigned_id = proc.pid;

      current_page++;
      remaining_mem -= list.page_size;
      
    }

    if (remaining_mem <= 0) break;
    
  }

  return list;

}

void print_list(frame_list list) {

  bool is_free = false;
  int start = 0;

  cout << "\tMemory map:\n";

  for (int i = 0; i < list.entries; i++) {
  
    if (!is_free && !list.frames[i].assigned) {
    
      is_free = true;
      start = i;

    }else if (is_free && list.frames[i].assigned) {

      is_free = false;
      cout << "\t\t" << start * list.page_size << "-"
	   << (i * list.page_size) - 1 << ": Free frame(s)\n";
    }
     
    if (list.frames[i].assigned) {

      cout << "\t\t" << i * list.page_size << "-"
	   << ((i + 1) * list.page_size) - 1 << ": Process"
           << list.frames[i].assigned_id << ", Page "
           << list.frames[i].pageNum << endl;
           
    }

  }
   
  if (is_free) {      
    cout << "\t\t" << start * list.page_size << "-"
	       << ((list.entries)* list.page_size) - 1 << ": Free frame(s)\n";
        
  }
}
