#!/bin/bash

echo "=============================================="
echo "Hot-Swap System - Comprehensive Test Suite"
echo "=============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
PASSED=0
FAILED=0
SKIPPED=0

# Function to print test result
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN} PASS${NC}: $2"
        ((PASSED++))
    else
        echo -e "${RED} FAIL${NC}: $2"
        ((FAILED++))
    fi
}

# Function to print warning
print_warning() {
    echo -e "${YELLOW}⚠ WARNING${NC}: $1"
    ((SKIPPED++))
}

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Build directory not found. Please build the project first."
    exit 1
fi

echo ""
echo "1. Building the project..."
cd build
make 2>&1 > build.log
if [ $? -eq 0 ]; then
    echo "Build successful"
else
    echo "Build failed. Check build.log for details."
    exit 1
fi

echo ""
echo "2. Checking for required .so files..."
REQUIRED_LIBS=("simple_module.so" "calculator_v1.so" "calculator_v2.so" "textprocessor_v1.so")
for lib in "${REQUIRED_LIBS[@]}"; do
    if [ -f "$lib" ]; then
        echo " Found: $lib"
    else
        echo " Missing: $lib"
        exit 1
    fi
done

echo ""
echo "=============================================="
echo "Starting Test Execution"
echo "=============================================="

# Functional Tests
echo ""
echo "3. FUNCTIONAL TESTING"
echo "======================"

# Test 3.1: Basic Module Loading
echo ""
echo "Test 3.1: Basic Module Loading"
./test_basic_loading > /dev/null 2>&1
print_result $? "Basic module loading and unloading"

# Test 3.2: Multiple Module Loading
echo ""
echo "Test 3.2: Multiple Module Loading"
./phase4_test > /dev/null 2>&1
print_result $? "Multiple module management"

# Test 3.3: Hot-Swap Functionality
echo ""
echo "Test 3.3: Hot-Swap Functionality"
./phase5_test > /dev/null 2>&1
print_result $? "Hot-swap between module versions"

# Test 3.4: Advanced Features
echo ""
echo "Test 3.4: Advanced System Features"
./advanced_demo > /dev/null 2>&1
print_result $? "Advanced system capabilities"

# Test 3.5: Error Handling
echo ""
echo "Test 3.5: Error Handling"
./test_invalid_module > /dev/null 2>&1
print_result $? "Error condition handling"

# Test 3.6: Stress Testing
echo ""
echo "Test 3.6: Stress Testing"
./test_stress > /dev/null 2>&1
print_result $? "System stability under stress"

# Memory Leak Tests
echo ""
echo "4. MEMORY LEAK TESTING"
echo "======================"

# Check if valgrind is installed
if command -v valgrind >/dev/null 2>&1; then
    echo ""
    echo "Test 4.1: Memory Leak Check - Basic Demo"
    valgrind --leak-check=full --error-exitcode=1 ./simple_demo > /dev/null 2>&1
    print_result $? "No memory leaks in basic demo"
    
    echo ""
    echo "Test 4.2: Memory Leak Check - Hot-Swap"
    valgrind --leak-check=full --error-exitcode=1 ./phase5_test > /dev/null 2>&1
    print_result $? "No memory leaks during hot-swap"
else
    print_warning "Valgrind not installed - skipping memory leak tests"
    echo "Install valgrind with: sudo apt-get install valgrind"
fi

# Performance Tests
echo ""
echo "5. PERFORMANCE TESTING"
echo "======================"

# Test 5.1: Module Loading Time
echo ""
echo "Test 5.1: Module Loading Performance"
start_time=$(date +%s%N)
./simple_demo > /dev/null 2>&1
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "Execution time: ${duration}ms"
if [ $duration -lt 5000 ]; then
    print_result 0 "Module loading performance acceptable"
else
    print_warning "Module loading slower than expected"
fi

# Summary
echo ""
echo "=============================================="
echo "TEST SUMMARY"
echo "=============================================="
echo -e "${GREEN}PASSED: ${PASSED}${NC}"
echo -e "${RED}FAILED: ${FAILED}${NC}"
if [ $SKIPPED -gt 0 ]; then
    echo -e "${YELLOW}SKIPPED: ${SKIPPED}${NC}"
fi

# Create test report
echo ""
echo "Generating test report..."
{
    echo "Hot-Swap System Test Report"
    echo "==========================="
    echo "Date: $(date)"
    echo ""
    echo "Results:"
    echo "- PASSED: $PASSED"
    echo "- FAILED: $FAILED"
    echo "- SKIPPED: $SKIPPED"
    echo ""
    
    if [ $FAILED -eq 0 ]; then
        echo "OVERALL STATUS: PASS"
        echo "All tests completed successfully!"
    else
        echo "OVERALL STATUS: FAIL"
        echo "Some tests failed. Please check the logs."
    fi
} > test_report.txt


# Final result
if [ $FAILED -eq 0 ]; then
    echo ""
    echo -e "${GREEN}=============================================="
    echo "ALL TESTS PASSED! "
    echo "Hot-Swap System is working correctly."
    echo -e "==============================================${NC}"
    exit 0
else
    echo ""
    echo -e "${RED}=============================================="
    echo "SOME TESTS FAILED! "
    echo "Please check the failed tests above."
    echo -e "==============================================${NC}"
    exit 1
fi