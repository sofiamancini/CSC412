# Assignment 2: UNIX Philosophy Implementation

## Overview
This assignment recreates Brian Kernighan's classic 1982 UNIX demo, demonstrating core UNIX principles through a pipeline of modular C programs and Bash scripts. The implementation showcases:
- Piping between programs
- Composition of simple utilities
- Efficient text processing
- C/Bash interoperability

## Key Features
- **Six modular C programs** forming processing pipeline
- **Bash scripts** demonstrating equivalent functionality
- **Extended FIO library** with file operations
- **Optimized algorithms** for text processing
- **Memory-efficient design**

## Programs in Pipeline
1. **sentence** - Reads input and formats into sentences
2. **makewords** - Splits sentences into words
3. **lowercase** - Normalizes case
4. **sort** - Alphabetizes words
5. **unique** - Removes duplicates
6. **mismatch** - Compares against dictionary

## Building and Running

### Prerequisites
- GCC compiler
- Bash shell
- Standard C library

### Quick Start

    # Make build script executable
    chmod +x build.sh

    # Compile all programs
    ./build.sh

    # Run full pipeline (C version)
    ./sentence | ./makewords | ./lowercase | ./sort | ./unique | ./mismatch

    # Run Bash equivalent
    ./sentence.sh | ./makewords.sh | ./lowercase.sh | ./sort.sh | ./unique.sh | ./mismatch.sh

### Build Options

    # Compile with debug symbols
    CFLAGS="-g" ./build.sh

    # Compile with optimizations
    CFLAGS="-O3" ./build.sh

## C Programs Implementation

| Program    | Key Algorithms           | Memory Management        |
|------------|--------------------------|--------------------------|
| sentence   | Buffered I/O             | Dynamic reallocation     |
| makewords  | Tokenization             | Pointer arithmetic       |
| lowercase  | ASCII manipulation       | In-place modification    |
| sort       | Hybrid sort (QSort+Insert) | External pointer array   |
| unique     | Linear scan              | In-place removal         |
| mismatch   | Binary search            | Memory-mapped I/O        |

## Performance Characteristics

| Operation          | Time Complexity | Space Complexity  | Notes                     |
|--------------------|-----------------|-------------------|---------------------------|
| Sentence parsing   | O(n)            | O(n)              | Buffered I/O              |
| Word tokenization  | O(n)            | O(n)              | Minimal copying           |
| Case conversion    | O(n)            | O(1)              | In-place modification     |
| Sorting            | O(n log n)      | O(n)              | Hybrid algorithm          |
| Deduplication      | O(n)            | O(1)              | Requires sorted input     |
| Dictionary lookup  | O(log n)        | O(1)              | Binary search             |

