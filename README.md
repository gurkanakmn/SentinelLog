<<<<<<< HEAD
![C++](https://img.shields.io/badge/C++-20-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)
# SentinelLog — Advanced C++ Logging Framework

A high-performance, **asynchronous**, thread-safe, and **runtime-configurable** logging engine built with **C++20**. Designed for modern applications where performance and reliability are critical.



## Key Features

- **Non-Blocking Architecture:** High-volume I/O operations are offloaded to a dedicated background worker thread, ensuring zero latency for your application's main logic.
- **Dynamic Hot-Reload:** Update logging thresholds, metadata toggles, and handler settings at runtime by editing `logger.conf`. The framework monitors file changes via a background watcher thread.
- **Incident Diagnostics (Black Box):** Maintains a circular memory buffer of recent logs. These are dumped to disk only when a specific error level (e.g., `ERROR`) is triggered, providing a "flight recorder" for your application.
- **C++20 source_location:** Automatically captures file names, line numbers, and function names without using old-school macros.
- **CrashGuard:** Robust signal handling (SIGSEGV, SIGABRT, etc.) that ensures all internal buffers are flushed to disk before the process terminates.
- **Multi-Format Support:** Flexible output strategies in **Standard Text**, **JSON**, or **CSV** formats.
- **Log Rotation:** Automated log file management with configurable size limits and backup counts.

---

## Project Structure

```text
LoggerFramework/
├── include/          # Public header files (Doxygen documented)
├── src/              # Implementation files (.cpp)
├── samples/          # Ready-to-run use case examples
│   ├── BasicUsage.cpp
│   ├── HotReloadDemo.cpp
│   └── IncidentReporting.cpp
├── logs/             # Generated log files
├── CMakeLists.txt    # Modern CMake build system
├── logger.conf       # Central configuration file
└── README.md         # Documentation
```
---

## Quick Test

```
#include "Logger.h"

int main() {
    auto& logger = Logger::getInstance();
    
    // Optional: Set a custom path before initialization
    logger.setConfigPath("config/custom_logger.conf");
    
    // Mandatory: Load configuration and start background threads
    logger.initConfig();

    logger.info("System initialized successfully.");
    logger.debug("Database connected to localhost:5432");
    
    return 0;
}
```

---

## Setup
### Create build directory

```
mkdir build && cd build
```

### Configure and Build

```
cmake ..
make
```

Note: Windows users are recommended to use Ninja or MinGW Makefiles.
NMake is not supported unless using the Visual Studio Developer Command Prompt.
### Run an example

```
./BasicUsage
```

---

## Configuration
The framework is governed by a logger.conf file, allowing you to control the environment without recompiling.
The settings explained in detail in config file.
### Sample Black Box Configuration (in .conf file)

```
=======
🛡️ Advanced C++ Logging FrameworkA high-performance, asynchronous, thread-safe, and runtime-configurable logging engine built with C++20. This framework is designed for modern C++ applications that require low-latency logging without sacrificing rich features like dynamic configuration and crash recovery.✨ Key FeaturesNon-Blocking Architecture: High-volume I/O operations are offloaded to a dedicated background worker thread, ensuring your application's main logic remains responsive.Dynamic Hot-Reload: Update logging thresholds, metadata toggles, and handler settings at runtime by editing logger.conf. The framework monitors file changes via a background watcher thread.Incident Diagnostics (Black Box): Features a circular memory buffer that records high-frequency DEBUG logs. These logs are only dumped to disk when a specific trigger level (e.g., ERROR) is reached, providing a "flight recorder" for your application.C++20 source_location: Leverages modern C++ to automatically capture file names, line numbers, and function names without the need for cumbersome preprocessor macros.Multi-Handler & Multi-Format: Support for multiple output destinations simultaneously (Console, Rolling File, Memory) with formatters for Plain Text, JSON, and CSV.CrashGuard: Robust signal handling (SIGSEGV, SIGABRT, etc.) to perform emergency flushes of all internal buffers before the process terminates.Log Rotation: Automated log file management with configurable size limits and backup counts to prevent disk exhaustion.📂 Project StructurePlaintextLoggerFramework/
├── include/          # Public header files (Doxygen documented)
├── src/              # Implementation files (.cpp)
├── samples/          # Ready-to-run use case examples
├── logs/             # Default directory for output logs
├── CMakeLists.txt    # Modern CMake build system
├── logger.conf       # Central configuration file
└── README.md         # You are here
🛠️ Build InstructionsThe project uses CMake 3.16+.Bash# 1. Clone the repository
git clone https://github.com/yourusername/LoggerFramework.git
cd LoggerFramework

# 2. Create build directory
mkdir build && cd build

# 3. Configure and Build
cmake ..
make

# 4. Run Examples
./BasicUsage
./HotReloadDemo
./IncidentReporting
🚀 Quick Start GuideIntegrating the logger into your project is straightforward. As a Singleton, it provides a global point of access.Basic IntegrationC++#include "Logger.h"

int main() {
    // 1. Get the global instance
    auto& logger = Logger::getInstance();

    // 2. Optional: Define custom config path (Default is logger.conf)
    logger.setConfigPath("path/to/my_config.conf");

    // 3. Mandatory: Initialize (Loads config and starts threads)
    logger.initConfig();

    // 4. Start Logging
    logger.info("Application started.");
    logger.debug("Database connected.");
    
    return 0;
}
⚙️ Configuration (logger.conf)The framework is highly customizable via the logger.conf file. Below are the primary configuration blocks:CategoryKeyDescriptionDefaultGlobalHOT_RELOAD_ENABLEDEnables live config monitoring.falseGlobalLOG_LEVELMinimum threshold (DEBUG to FATAL).INFOConsoleCOLOR_ENABLEDToggles ANSI colors for terminal.trueFileFILE_MAX_SIZEBytes before log rotation occurs.10MBMemoryMEMORY_CAPACITYSize of the circular "Black Box" buffer.500Sample Configuration snippet:Ini, TOML# Incident Diagnostics - Black Box Mode
>>>>>>> ea0e8cc (Initial commit: High-performance C++20 Logger Framework)
MEMORY_HANDLER_ENABLED=true
MEMORY_TRIGGER_LEVEL=ERROR
MEMORY_DUMP_PATH=../logs/incident_report.json
MEMORY_FORMAT=JSON
<<<<<<< HEAD
```

---

## Documentation
The project includes full Doxygen support for API references.

* Ensure doxygen and graphviz are installed.

* Run doxygen Doxyfile in the root directory.

* Open docs/html/index.html in your browser to view class hierarchies and call graphs.

---

## Requirements
* Compiler: GCC 10+, Clang 11+, or MSVC 2019+ (C++20 support required).

* Build System: CMake 3.16+

* OS: Linux, macOS, or Windows.

---

## License
This project is licensed under the MIT License.
=======
📖 Technical DocumentationThe project includes full Doxygen support. To generate the HTML documentation:Ensure doxygen and graphviz are installed.Run doxygen Doxyfile in the root directory.Open docs/html/index.html in your browser.🛠️ RequirementsCompiler: GCC 10+, Clang 11+, or MSVC 2019+ (with C++20 support).Dependencies: Standard Library only (pthreads required on Linux).Operating Systems: Windows, Linux, macOS.📝 LicenseDistributed under the MIT License. See LICENSE for more information.
>>>>>>> ea0e8cc (Initial commit: High-performance C++20 Logger Framework)
