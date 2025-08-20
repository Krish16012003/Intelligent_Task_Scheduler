#pragma once
#include <vector>
#include <list>
#include "Process.h"
#include "Core.h"

class Metrics {
public:
    static void printReport(const std::list<Process>& terminated_processes, 
                            const std::vector<Core>& cores, 
                            int total_cycles,
                            long long p_core_active_time,
                            long long e_core_active_time);
};