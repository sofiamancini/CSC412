
# CSC 412: A4 - Inklings

Sofia Mancini & Gina Vincent
Due: 20th March, 2025
Course: CSC 412 Operating Systems and Networks

## Introduction:
This document outlines the design and implementation details for an ASCII art-based terminal simulation where multi-threaded "inklings" traverse a grid while consuming resources. The project involves understanding existing code, implementing threading for inkling movement, logging inkling actions, and improving code efficiency and usability. The purpose of this assignment is to mimic what will often be encountered by anyone entering a new position. The concept is that being able to read and understand code given to you with little to no context is a crucial skill expected of software developers in a professional setting.

## Planning:

### Part 1: Understanding the ASCII Art Code
#### Overview:
This project includes an existing codebase that renders ASCII art in a terminal, which simulates a colored grid where mutiple threads will navigate randomly. The ascii_art.cpp program handles the rendering of the grid and visual updates.

#### Key Functions:
- ASCII Art Display: Creates a dynamic grid, where inklings move.
- Multithreading: Each inkling exists in its own thread, the grid updates concurrently.
- Resource Management: Tracks and limits ink availability.
- Keyboard Commands: ESC to terminate; r/g/b to refill ink.
- Thread Syncing: Ensures safe updates to the grid and resources to avoid race conditions and inconsistencies.

#### Plan:
1. Read and Understand the Codebase:
    - Become familiar with ascii_art.cpp.
    - Identify critical sections that should not be modified.
    - Understand how the grid is created and modified.
2. Test the Existing Code:
    - Run the program as is to observe behavior. 
    - Pass the stencial through Gradescope to observe errors.
    - Uncomment the std::thread listenerThread(keyListener); line to observe keyboard interactions.
    - Observe failures or unexpected outcomes.
3. Identify Potential Improvements:
    - Look for bugs in the syncing and locking.
    - Look for inefficiencies.

### Part 2: Creating Inklings as Multiple Threads
### Overview:
This part focuses on implementing multiple inkling threads that move randomly on the grid. Each inkling has a unique starting position, color, and movement constraints. The inklings consume ink resources and terminate when they reach a corner of the grid.

#### Key Functions:
- Inkling Types: Red, green, or blue.
- Movement: Randomly generated values within the grid boundaries.
- Resource Management: Inklings can only move with ink, they stop if ink runs out.
- Termination: Inklings terminate when they reach a grid corner.

#### Plan:
1. Implement Inkling Threads:
    - Write a function that handles inkling movement.
    - Use InklingInfo to store the data.
    - Check that the threads are being created and handled correctly.
2. Handle Movement:
    - Complete getNewDirection to generate random direction values within grid.
    - Implement checkIfInCorner to determine if inkling has reached a corner.
    - Write checkEnoughInk to check the resource level before moving.
3. Sync Threads:
    - Implement mutexes to protect resources.
    - Ensure access to the grid and ink is thread-safe
4. Testing:
    - Check that inklings move correctly and terminate at corners.
    - Ensure ink levels and grid positions update correctly.
    - Make sure inklings stop when ink level runs out.

## Part 3: Create Log Files for the Inklings
#### Overview:
Each inkling will have its own log file that tracks its actions (movement, termination, etc.). The logs will be stored in a logFolder directory and combined into a single actions.txt file sorted by timestamp.

#### Key Functions:
- Correct Log File Format: 
    - Timestamp, inkling ID, color, position, direction, and termination status.
    - Example: 21:20:30.562,inkling3,green,row3,col2.
- Log File Management:
    - Create a new directory logFolder.
    - Generate a log file for each inkling.
    - Combine and sort log files into actions.txt

#### Plan:
1. Implement Logging:
    - Modify the threadFunction to log the inkling actions.
    - Add timestamps.
2. Combine Log Files:
    - Write logs.cpp that will read in all log files, sort them by timestamp, and write to actions.txt

3. Testing:
    - Check that log files create correctly.
    - Check actions.txt is accurate.

### Part 4: Program Improvements
#### Overview:
This part involves proposing and implementing two improvements to the codebase. Potential areas for improvement include thread synchronization, keyboard event handling, grid representation, and logging efficiency.

#### Potential Sources of Improvement: 
1. Thread Synchronization:
    - Identify and fix bugs in thread synchronization.
    - Improve the efficiency of mutexes.
2. Keyboard Event Handling:
    - Ensure keyboard events are handled safely and efficiently.
    - Fix any failures or unexpected behavior when using keyboard commands.
3. Grid Representation:
    - Explore better data structures for representing the grid and inklings.
    - Improve the efficiency of grid updates.
4. Logging System:
    - Optimize the logging system to reduce overhead.
    - Add additional logging features (e.g., error logging).

#### Plan:
- Propose Improvements in improvements.md doc.
- Describe the problem, proposed solution, and expected benefits.
- Implement the proposed changes in the codebase.
- Test the changes to ensure they work as expected.
- Provide Unit and Integration Tests.

## Potential Difficulty
1. Understanding Codebase:
    - Reading an existing code can be difficult to comprehend.
    - Missing an error can lead to unexpected behaviour.
2. Thread Synchronization:
    - Ensuring thread-safe access to shared resources (e.g., ink levels, grid) will be difficult to see and test.
    - Debugging race conditions and deadlocks may be time-consuming.
3. Logging System:
    - Making sure the logs are accurate require careful planning.
    - Combining and sorting log files by timestamp can lead to many minute errors.

## Research
1. Thread Synchronization:
    - Research C++ mutexes and condition variables to ensure safe access to shared resources.
    - Study common pitfalls in multithreaded programming (e.g., deadlocks, race conditions).
2. Keyboard Event Handling:
    - Research how to safely handle keyboard events in a multithreaded environment.
    - Study best practices for event-driven programming.

## Rough Outline of Functions and Data Structures
### Functions:
- threadFunction(InklingInfo* inkling): Handles inkling movement and logging.
- getNewDirection(InklingInfo* inkling): Generates a random, valid direction.
- checkIfInCorner(InklingInfo* inkling): Checks if an inkling has reached a corner.
- checkEnoughInk(InklingInfo* inkling, int moveAmount): Verifies ink availability.
- redColorThreadFunc(), greenColorThreadFunc(), blueColorThreadFunc(): Refill ink levels.

### Data Structures:
- InklingInfo: Stores inkling data (color, position, direction, etc.).
- std::vector<InklingInfo>: Stores all inklings.
- std::mutex: Protects shared resources (e.g., ink levels, grid).