#pragma once
#include<vector>

using namespace std;

struct FRAME {
        int assigned;
        int proc_assign;
        int page_num;
};

struct frame_list {
        vector<FRAME> frames;
        int entries;
        int page_size;
};

frame_list create_frame_list(int entries, int page_size) {
        frame_list f;

        f.frames.resize(entries);

        f.page_size = page_size;
        f.entries = entries;

        for (int i = 0; i < f.entries; i ++) {
                f.frames[i].assigned = 0;
                f.frames[i].page_num = 0;
                f.frames[i].proc_assign = 0;
        }

        return f;
}

void print_frame_list(frame_list list) {
        bool in_free_block = false;
        int start = 0;

        cout << "\tMemory map:" << endl;

        for (int i = 0; i < list.entries; i++) {
                if (!in_free_block && !list.frames[i].assigned) {
                        in_free_block = true;
                        start = i;
                }
                else if (in_free_block && list.frames[i].assigned) {
                        in_free_block = false;
                        cout << "\t\t" << start * list.page_size
                                << "-" << (i * list.page_size) - 1
                                << ": Free frame(s)\n";
                }
                if (list.frames[i].assigned) {

                        cout << "\t\t" << i * list.page_size
                                << "-" << ((i + 1) * list.page_size) - 1
                                << ": Process"
                                << list.frames[i].proc_assign << ", Page "
                                << list.frames[i].page_num << endl;
                }

        }
        if (in_free_block) {
                cout << "\t\t" << start * list.page_size
                        << "-" << ((list.entries)* list.page_size) - 1
                        << ": Free frame(s)" << endl;
        }
}


frame_list free_memory_for_pid(frame_list list, int pid) {
        for (int i = 0; i < list.entries; i++) {
                if (list.frames[i].proc_assign == pid) {
                        list.frames[i].proc_assign = 0;
                        list.frames[i].page_num = 0;
                        list.frames[i].assigned = 0;
                }
        }
        return list;
}

int frame_list_is_empty(frame_list list) {
        for (int i = 0; i < list.entries; i++) {
                if (list.frames[i].assigned) {
                        return 0;
                }
        }
        return 1;
}

int valid_memory_check(frame_list list, PROCESS proc) {
        int num_free_frames = 0;

        for (int i = 0; i < list.entries; i++) {
                if (!list.frames[i].assigned) {
                        num_free_frames++;
                }
        }
        // if the number of free frames * the page size is greater than the mem req
        // for the process in question we can fit it in.
        return (num_free_frames * list.page_size) >= proc.request_memory_size;
}

frame_list add_process_to_queue_into_memory(frame_list list, PROCESS proc) {
        // this assumes you've already checked that you CAN fit the proc into mem
        int remaining_mem, current_page = 1;

        remaining_mem = proc.request_memory_size;

        for (int i = 0; i < list.entries; i++) {
                // if this frame is not assigned
                if (!list.frames[i].assigned) {
                        // assign it
                        list.frames[i].assigned = 1;
                        // set the page number
                        list.frames[i].page_num = current_page;
                        // set the proc num
                        list.frames[i].proc_assign = proc.pid;

                        current_page++;
                        //cout << list.frames[i].assigned << endl;
                        remaining_mem -= list.page_size;
                }

                if (remaining_mem <= 0) {
                        break;
                }
        }

        return list;
}

