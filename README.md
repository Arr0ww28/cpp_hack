# Smart Cabin & Vehicle Health Monitoring System

A production-grade, multi-threaded C++ Command Line Interface (CLI) application that simulates and monitors a vehicle's smart cabin and health systems. The application follows a service-oriented mindset, strictly utilizing modern C++ (C++17) concepts like RAII, smart pointers, multithreading, and STL containers without any external dependencies.

## Team Members
* Mohan Ram S
* Ramya Raman
* Harshad Reddy
* Tannishtha Nair
* Anish Chandel

---

## 🛠 Features and Modules

The system is highly modular and broken down into five core components:

### 1. Sensor Simulation Framework
Simulates 6 distinct sensors providing continuous or discrete data points:
* **Engine Temperature Sensor** (70°C – 120°C)
* **Battery Voltage Sensor** (9.5V – 14.8V)
* **Vehicle Speed Sensor** (0 km/h – 220 km/h)
* **Tire Pressure Sensor** (20 PSI – 40 PSI)
* **Door Status Sensor** (Open/Closed)
* **Seatbelt Sensor** (Locked/Unlocked)
* **How it works:** Each sensor employs a fine-grained mutex and generates simulated data using its own `std::mt19937` Random Number Generator. Can be toggled to a manual input mode via configuration.

### 2. Alert Management Module
Monitors sensor readings to raise and clear alerts dynamically. 
* Evaluates context-dependent conditions (e.g., Door Open is only a `CRITICAL` alert if the Vehicle Speed is > 10 km/h).
* Classifies alerts into `INFO`, `WARNING`, and `CRITICAL`.
* Maintains a thread-safe active alert list and a bounded historical alert deque.

### 3. Vehicle Dashboard Module
Renders a real-time, auto-refreshing ASCII-based dashboard in the terminal.
* Uses standard ANSI color codes (Green/Yellow/Red) without emojis or Unicode.
* Maintains live statistics (Min/Max/Avg) utilizing a map-based `VehicleStatistics` tracker.
* Provides interactive menu navigation using standard inputs.

### 4. Event Logger Module
A highly robust, asynchronous logging pipeline.
* Utilizes a lock-free `ThreadSafeQueue` producer-consumer model.
* Logs events out to `logs/vehicle_logs.txt`.
* Allows searching of in-memory log history using lambda-based predicates and `std::copy_if`.

### 5. Threading & Orchestration (Main)
Drives the concurrent execution pipeline using 4 background `std::thread`s and a main foreground thread:
* **Sensor Thread (500ms):** Generates new sensor readings.
* **Monitor Thread (750ms):** Evaluates sensor conditions and generates alerts.
* **Logger Thread (250ms):** Drains the async queue and writes to the log file.
* **Dashboard Thread (1000ms):** Auto-refreshes the active CLI view.
* **Main Thread:** Handles the interactive menu, user inputs, and graceful shutdown (SIGINT).

---

## 🚀 How to Build and Run

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
Upon launching, the interactive menu will appear. Use the numeric keys `1-7` to navigate the dashboard views.

---

## ⚙️ Configuration

The system behavior is governed by `data/config.txt`. You can modify this file to adjust thresholds, update intervals, and execution modes.

### Manual Debug Mode
To manually input sensor values for testing alerts:
1. Open `data/config.txt`.
2. Set `MANUAL_INPUT_MODE=1`.
3. Run `./vehicle_monitor`.
4. Choose option `[6] Manual Sensor Input` from the main menu.

---

## 🔍 Smart Log Viewer (`slog`)

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
