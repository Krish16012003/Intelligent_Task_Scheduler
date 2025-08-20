#pragma once
#include <vector>
#include <list>
#include "Process.h"
#include "Core.h"
#include "Metrics.h"

enum class SchedulerPolicy { NAIVE, HETEROGENEITY_AWARE, ADAPTIVE_AI };

class Scheduler {
public:
    Scheduler(int num_p_cores, int num_e_cores, SchedulerPolicy policy);
    void addProcess(Process p);
    void runSimulation(int cycles);
    void printResults();

private:
    std::vector<Core> cores;
    std::list<Process> all_processes;
    std::vector<Process*> ready_queue;
    
    int current_cycle;
    int total_simulation_cycles;
    SchedulerPolicy active_policy;

    long long p_core_total_active_time;
    long long e_core_total_active_time;

    void runCycle();
    void schedule();
    void handleNewArrivals();
    void updateRunningProcesses();
    void updateWaitingProcesses();
    void updateReadyQueue();
};