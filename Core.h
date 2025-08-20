#pragma once
#include "Process.h" 

enum class CoreType { PERFORMANCE, EFFICIENCY };

class Core {
public:
    int id;
    CoreType type;
    Process* current_process;
    double temperature;
    bool is_throttled;

    Core(int core_id, CoreType core_type)
        : id(core_id), type(core_type), current_process(nullptr), temperature(45.0), is_throttled(false) {}

    bool isIdle() const { return current_process == nullptr; }

    void assignProcess(Process* p) {
        if (isIdle()) {
            current_process = p;
            p->state = ProcessState::RUNNING;
        }
    }

    Process* releaseProcess() {
        Process* p = current_process;
        if (p != nullptr) {
            current_process = nullptr;
        }
        return p;
    }
};