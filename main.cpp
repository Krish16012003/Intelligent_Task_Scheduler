#include <iostream>
#include "Scheduler.h"
#include "Process.h"
#include "Core.h"

int main() {
    // --- Setup ---
    // Now you can test the ADAPTIVE_AI policy!
    SchedulerPolicy current_policy = SchedulerPolicy::ADAPTIVE_AI;

    Scheduler scheduler(2, 4, current_policy);

    // --- Process Creation ---
    // Note: We still provide the static_type for comparison purposes in the final report.
    scheduler.addProcess(Process(1, ProcessType::CPU_BOUND, 0, 20, 5, 1));
    scheduler.addProcess(Process(2, ProcessType::IO_BOUND,  1, 5,  25, 2));
    scheduler.addProcess(Process(3, ProcessType::CPU_BOUND, 2, 30, 10, 1));
    scheduler.addProcess(Process(4, ProcessType::IO_BOUND,  3, 8,  40, 3));
    scheduler.addProcess(Process(5, ProcessType::CPU_BOUND, 4, 15, 8, 2));
    scheduler.addProcess(Process(6, ProcessType::IO_BOUND,  5, 6,  30, 3));

    // --- Run Simulation ---
    scheduler.runSimulation(100);

    return 0;
}