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

### Docker (build & run)

If you prefer to build and run the project inside Docker the repository includes a `dockerfile` (lowercase) at project root. Example commands:

```bash
# Build the image (use BuildKit for faster, cached builds if available)
export DOCKER_BUILDKIT=1
docker build -t hotswap:latest -f dockerfile .

# Run the image (default CMD runs the demo copied into /app/build)
docker run --rm hotswap:latest

# If you see shared-library errors at runtime, run interactively to inspect files
docker run --rm -it --entrypoint bash hotswap:latest
# inside container: find /app -type f -name '*.so' -print

# If libraries are in /app/build you can set LD_LIBRARY_PATH when running
docker run --rm -e LD_LIBRARY_PATH=/app/build hotswap:latest

# Iterative development: mount source and build inside container to avoid
# rebuilding the whole image each edit
docker run --rm -it -v "$(pwd)":/app -w /app ubuntu:20.04 bash
# inside: apt-get update && apt-get install -y build-essential cmake ninja-build
# then build as usual: mkdir -p build && cd build && cmake -G Ninja .. && cmake --build . --target advanced_demo
```

Notes:
- The included `dockerfile` is a multi-stage builder that builds only the `advanced_demo` target and copies artifacts to the runtime stage. Adjust the Dockerfile if you want a different target.
- If you change the Dockerfile name to `Dockerfile` (capital D), you can omit `-f dockerfile` when calling `docker build`.
- Use `LD_LIBRARY_PATH` or `ENV LD_LIBRARY_PATH=/app/build` in the Docker image to ensure the dynamic loader finds `.so` files.

### Run Test

```bash
# Run comprehensive test suite
./tests/run_all_tests.sh

cd build/
# Run individual tests
./test_basic_loading
./test_stress

# Check for memory leaks
valgrind --leak-check=full ./simple_demo

# Check for memory errors
valgrind --tool=memcheck ./advanced_demo

# Performance Profiling
# Profile with gprof
cmake -DCMAKE_BUILD_TYPE=Release -DPROFILING=ON ..
make
./advanced_demo
gprof ./advanced_demo gmon.out > analysis.txt

# Profile with perf
perf record ./advanced_demo
perf report
```




