# Intelligent Task Scheduler for Heterogeneous Multi-Core Processors

This project is a C++ based simulation of an advanced operating system scheduler designed to manage processes on modern heterogeneous multi-core processors, such as those found in mobile and embedded System-on-Chip (SoC) architectures.

The simulation explores scheduling policies that balance **performance**, **power consumption**, and **thermal efficiency** by intelligently assigning tasks to either high-performance P-cores or power-saving E-cores.

---

### Key Features

* **Heterogeneous Architecture Simulation**: Models a multi-core system with distinct Performance-Cores (P-cores) and Efficiency-Cores (E-cores).
* **Adaptive AI Scheduling**: A key feature that dynamically classifies processes as either CPU-bound or I/O-bound based on their observed runtime behavior, allowing for intelligent, automated task placement.
* **Thermal Management**: Incorporates a thermal model where cores heat up under load, leading to performance throttling. This forces the scheduler to make complex decisions balancing performance and heat generation.
* **Comparative Analytics**: The simulation is designed to compare the performance of the intelligent scheduler against baseline and naive approaches using key metrics like turnaround time and simulated power consumption.

---

### Technology Stack

* **Language:** C++
* **Core Concepts:** Operating Systems, CPU Scheduling Algorithms, System Simulation, Process Management, Resource Management

---

### Current Status

* **[In Progress]** ⚙️ The core C++ simulation code is currently under development. The detailed project report outlining the design, architecture, and expected results is complete and available in the repository.

---

### How to Run (Once Complete)

1.  Clone the repository:
    `git clone <your-repository-link>`
2.  Navigate to the project directory:
    `cd intelligent-scheduler`
3.  Compile the C++ source code using a C++11 compliant compiler (e.g., g++):
    `g++ -std=c++11 -o scheduler_sim main.cpp`
4.  Run the simulation executable:
    `./scheduler_sim`
