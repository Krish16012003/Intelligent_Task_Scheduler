#pragma once

#include <iostream>
#include <string>

enum class ProcessState { NEW, READY, RUNNING, WAITING, TERMINATED };
enum class ProcessType { CPU_BOUND, IO_BOUND, UNKNOWN }; // UNKNOWN is the initial state for the AI scheduler

class Process {
public:
    int pid;
    ProcessState state;
    ProcessType static_type; // The "true" type of the process, used for comparison
    ProcessType dynamic_type; // The type determined by our AI scheduler
    int priority;
    
    // --- Metrics ---
    int arrival_time;
    int completion_time;
    int turnaround_time;
    int total_cpu_time;
    int total_io_time;
    int cpu_time_done;
    int io_time_done;
    int time_in_ready_queue;
    int wait_time;

    // **NEW**: Counters for AI classification
    int cycles_ran; // How many cycles the process has been observed for
    int cpu_burst_since_last_io; // Tracks CPU usage between I/O waits

    Process(int id, ProcessType p_type, int arrival, int cpu_time, int io_time, int prio)
        : pid(id), static_type(p_type), arrival_time(arrival), total_cpu_time(cpu_time),
          total_io_time(io_time), priority(prio) {
        
        state = ProcessState::NEW;
        dynamic_type = ProcessType::UNKNOWN; // Starts as unknown
        completion_time = -1;
        turnaround_time = -1;
        cpu_time_done = 0;
        io_time_done = 0;
        time_in_ready_queue = 0;
        wait_time = 0;
        cycles_ran = 0;
        cpu_burst_since_last_io = 0;
    }

    void printDetails() const {
        std::cout << "PID: " << pid 
                  << " | Priority: " << priority
                  << " | Type: " << (static_type == ProcessType::CPU_BOUND ? "CPU-Bound" : "I/O-Bound")
                  << " | CPU Time: " << cpu_time_done << "/" << total_cpu_time
                  << std::endl;
    }
};