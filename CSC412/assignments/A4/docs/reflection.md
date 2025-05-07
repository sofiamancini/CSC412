# Reflection on A4 - Inklings Implementation

## Overview

This reflection document provides a detailed review of the implementation process for the A4 - Inklings project. It outlines the changes made between the original design document and the final implementation, discusses what worked and what didn’t, highlights research efforts, estimates the time spent, and proposes a UNIT test for future validation.

## Changes Between Design Document and Final Implementation

### 1. Thread Synchronization Improvements
- Original Plan: The design document proposed improving thread synchronization to prevent race conditions and deadlocks.
- Final Implementation:
    - Introduced `std::atomic<bool> keepRunning` to control the lifecycle of the `keyListener` thread, ensuring it runs only once and terminates gracefully.
    - Added a static bool flag (`listenerThreadInitialized`) in `initializeFrontEnd()` to prevent duplicate thread creation.
    - These changes addressed the issue of duplicate threads and improved thread safety.

### 2. Terminal Mode Handling
- Original Plan: We did not anticipate the terminal handling issues
- Final Implementation:
    - Ensured `enableRawMode` and `disableRawMode` are called only once using a static bool flag.
    - Registered `disableRawMode` with `std::atexit` to ensure the terminal is restored to its initial state even on unexpected exits.
    - Replaced `std::cin` with `read(STDIN_FILENO, &c, 1)` for low-level input handling, improving input reliability.

### 3. Logging System
- Original Plan: The design document proposed creating individual log files for each inkling and combining them into a single `actions.txt` file.
- Final Implementation:
    - Implemented logging for inkling actions (movement, termination, etc.) with timestamps.
    - Created a `logFolder` directory to store individual log files.
    - Wrote a `logs.cpp` program to combine and sort log files by timestamp into `actions.txt`.

### 4. Keyboard Event Handling
- Original Plan: The design document identified keyboard event handling as a potential area for improvement.
- Final Implementation:
    - Improved keyboard event handling by ensuring `keyListener` runs in a dedicated thread.
    - Added synchronization to prevent mixing high-level and low-level input handling.

## What Worked and What Didn’t

### What Worked:
1. Thread Synchronization:
    - The introduction of `keepRunning` and `listenerThreadInitialized` successfully prevented duplicate threads and ensured thread safety.
2. Terminal Mode Handling:
    - Properly handling terminal modes improved input reliability and ensured the terminal is restored to its initial state.
3. Logging System:
    - The logging system worked as expected, creating individual log files and combining them into `actions.txt`.
4. Keyboard Event Handling:
    - Dedicated input-handling threads and proper synchronization resolved issues with keyboard inputs.

### What Didn’t Work:
1. Initial Thread Implementation:
    - The initial implementation of `keyListener` using `std::cin` caused undefined behavior and resource conflicts.
2. Terminal Mode Conflicts:
    - Initially, `enableRawMode` and `disableRawMode` were called multiple times, causing terminal mode conflicts.
3. Logging Overhead:
    - The logging system initially introduced significant overhead, which was later optimized.

## Research and Challenges

### Research:
1. Thread Synchronization:
    - Researched C++ mutexes and `std::atomic` to ensure safe access to shared resources.
    - Studied common pitfalls in multithreaded programming, such as deadlocks and race conditions.
2. Terminal Mode Handling:
    - Researched low-level terminal handling using `termios` and `read(STDIN_FILENO, &c, 1)`.
    - Studied best practices for restoring terminal state on program exit.
3. Logging System:
    - Researched file I/O in C++ to implement the logging system.
    - Studied timestamp generation and sorting algorithms for combining log files.

### Challenges:

1. Understanding the Codebase:
    - Reading and understanding the existing codebase was time-consuming, especially identifying critical sections that should not be modified.
2. Debugging Thread Issues:
    - Debugging race conditions and deadlocks required extensive testing and careful analysis.
3. Optimizing Logging:
    - Reducing the overhead of the logging system while maintaining accuracy was challenging.

## Time Spent
The assignment took approximately 25 hours to complete, broken down as follows:

- Understanding the Codebase: 5 hours  
- Implementing Thread Synchronization: 4 hours
- Improving Terminal Mode Handling: 6 hours
- Implementing the Logging System: 1 hours
- Testing and Debugging: 9 hours

## UNIT Test Proposal
### Purpose:
To verify that the `keyListener` thread correctly handles keyboard inputs and terminates gracefully.

### Input Data:
- Simulate keyboard inputs (r, g, b, +, -, <, >, ESC).
- Trigger abrupt termination (e.g., Ctrl+C).

### Expected Output:
- The program should respond correctly to keyboard inputs.
- The terminal should be restored to its initial state after program exit.
- No duplicate threads should be created.

### Test Procedure:
1. Run the program.
2. Simulate keyboard inputs using the test script.
3. Verify that the program responds correctly to inputs.
4. Terminate the program abruptly and verify that the terminal is restored to its initial state.
5. Check the log files to ensure no duplicate threads were created.

### Explanation:
This test confirms the correctness of the `keyListener` thread and ensures proper terminal mode handling. It also verifies that the program cleans up resources correctly, even on unexpected exits.