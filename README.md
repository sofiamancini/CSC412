# CSC412: Operating Systems and Networks

**Repository for course assignments and labs**  
*University of Rhode Island*  
*Term: Spring 2025*

## Course Description
This repository contains my work for CSC412 - Operating Systems and Networks. The course covers fundamental concepts including process management, memory organization, file systems, networking protocols, and concurrent programming.

## Technologies Used
- **Primary Languages**: C, C++
- **Secondary Languages**: Go, Bash
- **Tools**: Docker, Makefiles, gcc/g++, git
- **Concepts**: System calls, sockets, threading, memory management


## How to Approach This Repository
1. Each assignment/lab has its own directory with source code
2. Most projects include a Makefile for building
3. Check individual assignment READMEs for specific instructions

## Assignment Overview

| Assignment | Description | Technologies |
|------------|-------------|--------------|
| [A1: String & Sentence Reversal](/CSC412/assignments/A1) | Implemented 5 string reversal methods and 5 sentence reversal algorithms in C. Included Bash scripting for automated testing. | C, Bash |
| [A2: UNIX Philosophy Implementation](/CSC412/assignments/A2) | Recreated classic UNIX demo programs demonstrating piping and modular design. Implemented I/O operations in both C and Bash. | C, Bash, UNIX Tools |
| [A3: Custom Shell & Utilities](/CSC412/assignments/A3) | Developed a shell program using fork/execv system calls. Recreated GNU utilities in C++ and Rust (dual implementations). | C, C++, Rust, Makefile |
| [A4: Multithreaded Codebase Improvements](/CSC412/assignments/A4) | Enhanced existing C++ codebase with thread synchronization (mutexes), file handling, and debugging systems. Added logging functionality. | C++, Multithreading, File I/O |
| [A5: Snake Game with Memory Management](/CSC412/assignments/A5) | Implemented classic Snake game with dynamic memory allocation, ASCII/Unicode support, and custom level system. Focused on pointer manipulation. | C, Dynamic Memory, Unicode |
| [A6: Debug Memory Allocator](/CSC412/assignments/A6) | Built custom memory allocator with leak detection, overflow protection, and statistics tracking. Implemented safety checks against common memory errors. | C, Memory Debugging, Security |




## Getting Started
```bash
git clone https://github.com/sofiamancini/CSC412.git
cd CSC412/assignments/assignment1
make
./bin/program

