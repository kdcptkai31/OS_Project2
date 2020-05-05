#pragma once
class PROCESS{
public:
        int pid;
        int arriving_time;
        int burst_time;
        int request_memory_size;

        int time_added_to_memory;
        int is_active;
        int time_finished;
};

