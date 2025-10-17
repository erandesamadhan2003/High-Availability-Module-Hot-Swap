# High-Availability Module Hot-Swap System

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.15+-green.svg)
![Linux](https://img.shields.io/badge/Linux-Compatible-orange.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

A high-performance C++ system for runtime module replacement without downtime. Enables zero-downtime updates, automatic health monitoring, and dynamic module management.

## 🚀 Key Features

- **Hot-Swapping**: Replace modules at runtime without system restart
- **Health Monitoring**: Automatic health checks and performance metrics
- **Comprehensive Logging**: File and console logging with multiple levels
- **Dynamic Loading**: Load/unload modules from shared libraries (.so files)
- **Thread-Safe**: Built with thread safety for concurrent operations
- **Performance Metrics**: Track load times, failure rates, and uptime
- **Error Handling**: Graceful degradation and automatic recovery



## 📦 Quick Start

### Prerequisites
- C++17 compatible compiler (GCC 9+ or Clang 10+)
- CMake 3.15+
- Linux environment

### Build Instructions
```bash
# Clone the repository
git clone <repository-url>
cd High-Availability-Module-Hot-Swap

# Build the project
mkdir build && cd build
cmake ..
make

# Run the demo
./simple_demo
```

### Run Test

```bash
# Run comprehensive test suite
./tests/run_all_tests.sh

# Run individual tests
./test_basic_loading
./test_stress
```


