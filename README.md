# Process Management Monitor

A comprehensive, interactive Linux process monitoring and management tool written in C++. This project demonstrates low-level system interactions by directly parsing the Linux Virtual File System (`/proc`) and utilizing Linux System Calls to manage running processes. 

Developed as part of the Operating Systems coursework.

---

## Core Features

*   **Real-Time Telemetry:** Continuously monitors CPU utilization, memory consumption (RAM), and process states by reading from `/proc/[pid]/stat` and `/proc/[pid]/status`.
*   **Terminal User Interface (TUI):** A responsive, `htop`-like interface built with the `ncurses` library for seamless navigation.
*   **Active Process Control:** Send POSIX signals directly to processes (e.g., `SIGKILL`, `SIGSTOP`, `SIGCONT`) via the interface.
*   **Process Hierarchy Visualization:** Maps and displays parent-child process relationships (PPID to PID) in a structured tree format.
*   **Dynamic Sorting:** Sort processes dynamically by CPU usage, Memory consumption, or PID.

---

## System Calls & OS Concepts Utilized

This project heavily relies on core Operating System principles and Linux APIs:

1.  **Virtual File System (VFS):** 
    *   Parses `/proc/stat` for total system CPU ticks.
    *   Parses `/proc/[pid]/stat` for process-specific execution times and states (Running, Sleeping, Zombie).
2.  **Inter-Process Communication (Signals):** 
    *   Utilizes the `kill(pid_t pid, int sig)` system call to send signals to running processes.
3.  **Process Context:** 
    *   Reads `/proc/[pid]/status` to extract exact memory pages allocated to the process context.
4.  **Concurrency & Threading:** 
    *   (Optional) Utilizes `pthreads` to separate the UI rendering loop from the data-fetching loop, ensuring a smooth interface even under heavy load.

---

## Prerequisites & Installation

Ensure your Linux environment has the following development libraries installed:

```bash
# Update package list
sudo apt update

# Install GCC/G++ Compiler, Make, and ncurses library
sudo apt install build-essential libncurses5-dev libncursesw5-dev
```

---

## Build and Execution

**1. Clone and Navigate**
Extract the project ZIP file (e.g., `Group_Name_Project2.zip`) and navigate to the root directory.

**2. Compile the Project**
Use the provided `Makefile` to compile the source code automatically.
```bash
make
```

**3. Run the Application**
Execute the compiled binary. *Note: Monitoring or terminating system-level processes requires elevated privileges.*
```bash
sudo ./process_monitor
```

**4. Clean Build**
To remove compiled object files and clean the directory:
```bash
make clean
```

---

## Usage & Keybindings

| Key | Action | POSIX Signal / Description |
| :--- | :--- | :--- |
| `↑` / `↓` | **Navigate** | Move the selector up or down the process list. |
| `k` | **Kill** | Sends `SIGKILL` (9) to forcefully terminate the process. |
| `t` | **Terminate** | Sends `SIGTERM` (15) for a graceful shutdown. |
| `s` | **Suspend** | Sends `SIGSTOP` (19) to pause process execution. |
| `c` | **Continue** | Sends `SIGCONT` (18) to resume a suspended process. |
| `h` | **Hierarchy** | Toggle between Standard List View and Tree View. |
| `q` | **Quit** | Exit the process monitor gracefully. |

---

## Directory Structure

```text
Process-Monitor-Project/
├── src/                    
│   ├── main.cpp            # Application entry point and TUI event loop
│   ├── proc_parser.cpp     # Logic for extracting/parsing /proc data
│   ├── process_ctrl.cpp    # System call implementations (kill, signal routing)
│   └── tui_display.cpp     # ncurses-based rendering logic
├── include/                
│   ├── proc_parser.hpp
│   ├── process_ctrl.hpp
│   └── tui_display.hpp
├── docs/                   
│   └── Presentation.pdf    # 10-Minute English Presentation Slides
├── Makefile                # Build configuration and linking rules
└── README.md               # Project documentation
```

---

## Team Members

| Student ID | Name | Project Role & Contributions |
| :--- | :--- | :--- |
| `6730300116` | `Charin Phuaphumcharoen` | **Data Extraction & Parser:** Handled file I/O operations for the `/proc` directory and string tokenization. |
| `6730300051` | `Krairawee Boonthad` | **Process Metrics & Logic:** Implemented CPU/RAM calculation algorithms and the Process Hierarchy tree sorting. |
| `6730300574` | `Siwakorn Pratumsuwan` | **TUI Developer:** Designed the real-time interactive dashboard using the `ncurses` library. |
| `6730300701` | `Kittikanoot Sangiam` | **Process Controller:** Integrated Linux system calls for process management and signal handling. |
| `6730300175` | `Soungwut Konak` | **QA, Integration & Demo Master:** Managed code integration, Makefile setup, and prepared the final English presentation. |