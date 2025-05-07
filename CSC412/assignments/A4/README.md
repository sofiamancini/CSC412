# Assignment 4: Inklings - Multithreaded ASCII Art Simulation

## Overview
This project implements a terminal-based ASCII art simulation where multithreaded "inklings" navigate a grid while consuming color resources. The implementation focuses on thread synchronization, logging, and improving an existing codebase.

## Key Features
- **Multithreaded inkling movement**: Each inkling operates in its own thread
- **Resource management**: Tracks and limits ink availability
- **Comprehensive logging**: Individual inkling logs combined into timestamp-sorted file
- **Terminal UI**: Dynamic grid display with keyboard controls
- **Thread-safe operations**: Protected resource access and grid updates

## Building and Running

### Prerequisites
- C++20 compatible compiler
- POSIX-compliant terminal

### Quick Start
```bash
# Build both main program and log processor
make

# Run the simulation
make run

# Process logs (after running simulation)
make run-logs

# Clean build artifacts
make clean
```

## Implementation Details

### Core Components

```cpp
// Thread function for each inkling
void threadFunction(InklingInfo* inkling) {
    while (!checkIfInCorner(inkling)) {
        if (checkEnoughInk(inkling, moveAmount)) {
            moveInkling(inkling);
            logMovement(inkling);
        }
    }
    logTermination(inkling);
}
```

### Thread Synchronization Improvements

Atomic flag for thread control:

```cpp
std::atomic<bool> keepRunning{true};
```

Single thread initialization:

```cpp
static bool listenerThreadInitialized = false;
if (!listenerThreadInitialized) {
    listenerThread = std::thread(keyListener);
    listenerThreadInitialized = true;
}
```

Terminal mode safety:

```cpp
std::atexit(disableRawMode);
```

### Performance Characteristics
| Operation       | Time Complexity | Thread Safety       |
|-----------------|-----------------|--------------------|
| Inkling movement| O(1) per move  | Mutex-protected     |
| Grid update     | O(n) for n inklings | Atomic operations |
| Log writing     | O(1) per entry | Individual files    |


## Key Improvements Implemented

### 1. Thread Management System

**Problem:** Uncontrolled thread creation led to resource conflicts.  
**Solution:**

```cpp
extern std::atomic<bool> keepRunning;

void keyListener() {
    while (keepRunning) {
        char c;
        if (read(STDIN_FILENO, &c, 1) > 0) {
            myKeyboard(c);
        }
    }
}
```

**Benefits:**
- Prevents duplicate threads
- Ensures clean thread termination
- Reduces resource contention

### 2. Terminal Mode Handling

**Problem:** Terminal state corruption on unexpected exits.  
**Solution:**

```cpp
void initializeFrontEnd() {
    static bool initialized = false;
    if (!initialized) {
        enableRawMode();
        std::atexit(disableRawMode);
        initialized = true;
    }
}
```

**Benefits:**
- Guaranteed terminal state restoration
- Single initialization point
- Crash-safe cleanup

## Logging System

```
21:20:30.562,inkling3,green,row3,col2,up,active
21:20:31.120,inkling3,green,row2,col2,up,terminated
```

- Individual log files per inkling
- Combined and sorted by timestamp
- Includes position, direction, and status

## Testing Approach

### Unit Tests
- **Thread Safety:**

```bash
./inklings --test-threads
```

- **Logging Validation:**

```bash
./logs --validate
```

### Integration Test

```bash
# Full simulation test
./inklings --rows 20 --cols 40 --ink 500

# Verify output in logFolder/actions.txt
```

---
