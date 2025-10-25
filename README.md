# Aerospace-OS

A lightweight ARINC‑653 simulator and avionics protocol implemented in portable C for Linux/POSIX.  
Designed for experimentation, and demonstrations of real‑time partitioning, IPC services, and representative ARINC/MIL‑STD avionics protocol utilities.

---

## Highlights
- Implements a static cyclic major‑frame scheduler and POSIX‑based process model that simulates ARINC‑653 partitioning and temporal isolation.
- Provides IPC primitives: sampling ports (latest‑value) and queuing ports (bounded FIFO).
- Includes representative protocol helpers: ARINC‑429 encoder/decoder, AFDX (ARINC‑664) scaffolding, MIL‑STD‑1553B helpers.
- Clean C11 codebase structured for incremental extension and unit testing.
- Build with CMake; runs on Linux (POSIX threads and monotonic clocks).

---

## Quick architecture overview
- Scheduling & partition model.
- Partition services:
  - Partition lifecycle.
  - Scheduler: cyclic major frame.
- Process management:
  - Start/stop processes.
- IPC primitives:
  - Sampling ports.
  - Queuing ports.
- Protocol helpers:
  - ARINC‑429.
  - AFDX (ARINC‑664).
  - MIL‑STD‑1553B.

---

## Key files & entry point
- Build configuration: [CMakeLists.txt]
- Example launcher: [Runner/main.c] — boots a 2‑partition example, spawns processes via [`start_process`]and runs the scheduler via [`run_scheduler`].
- Sample partition tasks: [Task2], [Task2].

---

## Build (local dev / CI)
1. Configure & build:
   ```sh
   mkdir -p build
   cd build
   cmake ..
   cmake --build .
