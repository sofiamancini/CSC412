# Assignment 1: String and Sentence Reversal

## Overview
This assignment implements multiple approaches to reversing both individual strings and complete sentences in C. The solution includes:
- 5 distinct string reversal algorithms
- 5 sentence reversal methods
- A test harness to verify functionality
- Bash build automation

## Implementation Details

### String Reversal Methods
1. **Iterative Pointer Arithmetic**: Uses pointer manipulation to reverse in-place
2. **Stack-Based**: Utilizes a LIFO stack structure
3. **Recursive**: Demonstrates recursive string reversal
4. **Half-Swap**: Optimized swapping algorithm
5. **XOR Bitwise**: Low-level bit manipulation approach

### Sentence Reversal Methods
1. **Word Stack**: Reverses word order using a stack
2. **In-Place Word Rotation**: Modifies sentence buffer directly
3. **Recursive Word Parse**: Recursive word order reversal
4. **Token-Based**: Uses strtok for word separation
5. **Double Reverse**: Full reverse followed by word reverse

## Building and Running

### Prerequisites
- GCC compiler
- Bash shell (for build script)
- GNU Make (optional)

### Quick Start
    
    # Make the build script executable
    chmod +x bash.sh

    # Build and run with test input
    ./bash.sh "your test string here"

    # Alternatively, run directly after building
    ./revtest "your test string here"


### Build Script Usage

The bash.sh script handles compilation and execution:

    # Basic usage
    ./bash.sh "input string"

    # Run all test cases
    ./bash.sh --test

    # Show help
    ./bash.sh --help

## Test Cases
The program includes built-in verification for:
- Empty strings
- Single characters
- Palindromes
- Unicode characters
- Multiple whitespace patterns
- Mixed-case strings

## Design Considerations

- All functions preserve original string immutability
- Memory safety verified with Valgrind
- Includes boundary condition handling
- UTF-8 compatible implementations

## Performance Characteristics

| Method          | Time Complexity | Space Complexity | Notes                     |
|-----------------|-----------------|-------------------|---------------------------|
| Iterative       | O(n)            | O(1)              | Most memory efficient     |
| Recursive       | O(n)            | O(n)              | Stack frame overhead      |
| Stack-Based     | O(n)            | O(n)              | Clean but uses extra mem  |
| Half-Swap       | O(n)            | O(1)              | Optimized for even lengths|
| XOR             | O(n)            | O(1)              | No temp variable needed   |