#include "Scheduler.h"
#include <iostream>
#include <algorithm>
#include <numeric>

Scheduler::Scheduler(int num_p_cores, int num_e_cores, SchedulerPolicy policy) 
    : current_cycle(0), total_simulation_cycles(0), active_policy(policy),
      p_core_total_active_time(0), e_core_total_active_time(0) {
    for (int i = 0; i < num_p_cores; ++i) cores.emplace_back(i, CoreType::PERFORMANCE);
    for (int i = 0; i < num_e_cores; ++i) cores.emplace_back(num_p_cores + i, CoreType::EFFICIENCY);
}

void Scheduler::addProcess(Process p) { all_processes.push_back(p); }

void Scheduler::runSimulation(int cycles) {
    total_simulation_cycles = cycles;
    for (int i = 0; i < cycles; ++i) {
        current_cycle = i;
        runCycle();
    }
    printResults();
}

void Scheduler::runCycle() {
    std::cout << "\n--- Cycle " << current_cycle << " ---" << std::endl;
    handleNewArrivals();
    updateRunningProcesses();
    updateWaitingProcesses();
    updateReadyQueue();
    schedule();
}

void Scheduler::handleNewArrivals() {
    for (auto& p : all_processes) {
        if (p.arrival_time == current_cycle && p.state == ProcessState::NEW) {
            p.state = ProcessState::READY;
            std::cout << "PID " << p.pid << " has arrived." << std::endl;
        }
    }
}

void Scheduler::updateRunningProcesses() {
    for (auto& core : cores) {
        if (!core.isIdle()) {
            Process* p = core.current_process;
            p->cpu_time_done++;
            p->cycles_ran++;
            p->cpu_burst_since_last_io++;

            if (core.type == CoreType::PERFORMANCE) p_core_total_active_time++;
            else e_core_total_active_time++;

            if (p->cpu_time_done >= p->total_cpu_time) {
                p->state = ProcessState::TERMINATED;
                p->completion_time = current_cycle + 1;
                p->turnaround_time = p->completion_time - p->arrival_time;
                core.releaseProcess();
                std::cout << "PID " << p->pid << " has terminated." << std::endl;
            } 
            else if (p->io_time_done < p->total_io_time && (p->cpu_burst_since_last_io >= 5)) { 
                p->state = ProcessState::WAITING;
                p->cpu_burst_since_last_io = 0; // Reset burst counter
                core.releaseProcess();
                std::cout << "PID " << p->pid << " moved to WAITING for I/O." << std::endl;
            }
        }
    }
}

void Scheduler::updateWaitingProcesses() {
    for (auto& p : all_processes) {
        if (p.state == ProcessState::WAITING) {
            p.io_time_done++;
            p.wait_time++;
            if (p.io_time_done >= p.total_io_time) {
                p.state = ProcessState::READY;
                std::cout << "PID " << p.pid << " finished I/O, moved back to READY." << std::endl;
            }
        }
    }
}

void Scheduler::updateReadyQueue() {
    ready_queue.clear();
    for (auto& p : all_processes) {
        if (p.state == ProcessState::READY) {
            ready_queue.push_back(&p);
            p.wait_time++;
        }
    }
    std::sort(ready_queue.begin(), ready_queue.end(), [](const Process* a, const Process* b) {
        return a->priority < b->priority;
    });
}

void Scheduler::schedule() {
    std::cout << "Running scheduling logic with policy: ";
    switch(active_policy) {
        case SchedulerPolicy::NAIVE:
            // ... (code is unchanged)
            std::cout << "NAIVE" << std::endl;
            for (auto& core : cores) {
                if (core.isIdle() && !ready_queue.empty()) {
                    Process* p = ready_queue.front();
                    ready_queue.erase(ready_queue.begin()); 
                    core.assignProcess(p);
                    std::cout << "Assigned PID " << p->pid << " to Core " << core.id << std::endl;
                }
            }
            break;

        case SchedulerPolicy::HETEROGENEITY_AWARE:
            // ... (code is unchanged)
            std::cout << "HETEROGENEITY_AWARE" << std::endl;
            for (auto it = ready_queue.begin(); it != ready_queue.end(); ) {
                Process* p = *it;
                bool assigned = false;
                if (p->static_type == ProcessType::CPU_BOUND) {
                    for (auto& core : cores) {
                        if (core.type == CoreType::PERFORMANCE && core.isIdle()) {
                            core.assignProcess(p); assigned = true;
                            std::cout << "Assigned CPU-Bound PID " << p->pid << " to P-Core " << core.id << std::endl;
                            break;
                        }
                    }
                } else { // IO_BOUND
                    for (auto& core : cores) {
                        if (core.type == CoreType::EFFICIENCY && core.isIdle()) {
                            core.assignProcess(p); assigned = true;
                            std::cout << "Assigned I/O-Bound PID " << p->pid << " to E-Core " << core.id << std::endl;
                            break;
                        }
                    }
                }
                if (assigned) it = ready_queue.erase(it);
                else ++it;
            }
            break;

        case SchedulerPolicy::ADAPTIVE_AI:
            std::cout << "ADAPTIVE_AI" << std::endl;
            for (auto it = ready_queue.begin(); it != ready_queue.end(); ) {
                Process* p = *it;
                bool assigned = false;

                // **AI LOGIC STARTS HERE**
                // 1. Observation and Classification
                if (p->dynamic_type == ProcessType::UNKNOWN && p->cycles_ran > 3) { // Observe for 3 cycles
                    double cpu_ratio = (double)p->cpu_time_done / p->cycles_ran;
                    if (cpu_ratio > 0.7) {
                        p->dynamic_type = ProcessType::CPU_BOUND;
                        std::cout << "AI classified PID " << p->pid << " as CPU-BOUND." << std::endl;
                    } else {
                        p->dynamic_type = ProcessType::IO_BOUND;
                        std::cout << "AI classified PID " << p->pid << " as I/O-BOUND." << std::endl;
                    }
                }

                // 2. Scheduling based on dynamic type
                if (p->dynamic_type == ProcessType::CPU_BOUND) {
                    // Prefer P-core
                    for (auto& core : cores) {
                        if (core.type == CoreType::PERFORMANCE && core.isIdle()) {
                            core.assignProcess(p); assigned = true;
                            std::cout << "Assigned CPU-Bound PID " << p->pid << " to P-Core " << core.id << std::endl;
                            break;
                        }
                    }
                } else { // IO_BOUND or UNKNOWN
                    // Prefer E-core for I/O bound or initial placement
                    for (auto& core : cores) {
                        if (core.type == CoreType::EFFICIENCY && core.isIdle()) {
                            core.assignProcess(p); assigned = true;
                            std::cout << "Assigned I/O-Bound/Unknown PID " << p->pid << " to E-Core " << core.id << std::endl;
                            break;
                        }
                    }
                }
                
                // If preferred core is busy, try any core (to prevent starvation)
                if(!assigned) {
                    for (auto& core : cores) {
                        if (core.isIdle()) {
                            core.assignProcess(p); assigned = true;
                            std::cout << "Assigned PID " << p->pid << " to fallback Core " << core.id << std::endl;
                            break;
                        }
                    }
                }

                if (assigned) it = ready_queue.erase(it);
                else ++it;
            }
            break;
    }
}

void Scheduler::printResults() {
    std::list<Process> terminated;
    for(const auto& p : all_processes) {
        if (p.state == ProcessState::TERMINATED) {
            terminated.push_back(p);
        }
    }
    Metrics::printReport(terminated, cores, total_simulation_cycles, p_core_total_active_time, e_core_total_active_time);
}