# Smart Cabin & Vehicle Health Monitoring System

A production-grade, multi-threaded C++ Command Line Interface (CLI) application that simulates and monitors a vehicle's smart cabin and health systems. The application follows a service-oriented mindset, strictly utilizing modern C++ (C++17) concepts like RAII, smart pointers, multithreading, and STL containers without any external dependencies.

## Team Members
* Mohan Ram S
* Ramya Raman
* Harshad Reddy
* Tannishtha Nair
* Anish Chandel

---

## Features and Modules

The system is highly modular and broken down into advanced core components:

### 1. Sensor Simulation & Manual Input
Simulates 6 distinct sensors providing continuous or discrete data points:
* **Engine Temperature Sensor** (70°C – 120°C)
* **Battery Voltage Sensor** (9.5V – 14.8V)
* **Vehicle Speed Sensor** (0 km/h – 220 km/h)
* **Tire Pressure Sensor** (20 PSI – 40 PSI)
* **Door Status Sensor** (Open/Closed)
* **Seatbelt Sensor** (Locked/Unlocked)
* **How it works:** A background `SimThread` generates smooth, realistic data using time-based sinusoidal waves (from `<cmath>`) instead of erratic random numbers. You can also seamlessly toggle Auto-Simulation off and manually input specific values to test edge cases!

### 2. Adaptive Alert Prioritization
Monitors sensor readings to raise and clear alerts dynamically.
* Evaluates context-dependent conditions (e.g., Door Open is only a `CRITICAL` alert if the Vehicle Speed is > 10 km/h).
* Classifies alerts into `INFO`, `WARNING`, and `CRITICAL`.
* **Adaptive Priority:** Utilizes an STL `std::priority_queue` (Max-Heap) in both the async EventLogger and the Dashboard UI. This guarantees that `CRITICAL` safety alerts immediately bypass standard informational messages and bubble to the very top.

### 3. Driver Profile Management
* Dynamic runtime configurations based on loaded driver profiles (Default, Eco, Sport, Performance).
* Switching to a "Sport" profile instantly raises the engine temperature warning threshold and speed limit, automatically suppressing false alarms while driving aggressively.

### 4. Vehicle Dashboard Module
Renders a real-time, auto-refreshing ASCII-based dashboard in the terminal.
* Uses standard ANSI color codes (Green/Yellow/Red) without emojis or Unicode.
* **Live Refresh:** Provides interactive menu navigation, including a Live Dashboard where you can press `[R]` to instantly refresh or `[Q]` to toggle Auto-Simulation on the fly.
* Maintains live statistics (Min/Max/Avg) utilizing a map-based `VehicleStatistics` tracker.

### 5. Watchdog Health Monitor
* A dedicated background supervisor that tracks the lifecycle of all other running threads.
* Every module (Sensors, Dashboard, Logger) publishes a "heartbeat" to the `HealthMonitor`. If a thread hangs (like the LoggerThread), the Watchdog detects the missing heartbeat and instantly publishes an `ECU_FAULT` alert to the system.

### 6. Event Logger Module
A highly robust, asynchronous logging pipeline.
* Utilizes a lock-free `ThreadSafeQueue` producer-consumer model (now upgraded to prioritize `CRITICAL` events).
* Logs events out to `logs/vehicle_logs.txt`.
* Allows searching of in-memory log history using lambda-based predicates and `std::copy_if`.

---

## How to Build and Run

### Prerequisites
* A C++17 compatible compiler (e.g., GCC or Clang).
* Make

### Building the Project
Simply run `make` at the root of the project to build the executable:
```bash
make clean
make
```

### Running the Application
```bash
./vehicle_monitor
```
Upon launching, the interactive menu will appear. Use the numeric keys `1-8` to navigate the dashboard views. To see the vehicle in action:
1. Go to `[6] Manual Sensor Input`.
2. Press `S` to toggle Auto-Simulation **ON**.
3. Go to `[1] Live Dashboard` and mash `R` to watch the data realistically flow!

---

## Configuration

The system behavior is governed by `data/config.txt` and `profiles.json`. You can modify these files to adjust baseline thresholds, update intervals, and execution modes.

---

## Smart Log Viewer (`slog`)

A custom bash script is provided to easily search and tail the output logs (`logs/vehicle_logs.txt`).

**Ensure the script is executable:**
```bash
chmod +x slog
```

**Usage Examples:**
```bash
./slog                     # Show all logs
./slog -f                  # Tail the log in real-time (like tail -f)
./slog -n 30               # Show the last 30 log entries
./slog CRITICAL            # Show only CRITICAL level alerts
./slog "ENGINE OVERHEAT"   # Search for specific log phrases
./slog --clear             # Clear the current log file
```
