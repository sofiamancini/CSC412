# Reflection on A6: DMalloc

Sofia Mancini
Due: 24th April, 2025

## Overview

This reflection will document the development process of the debugging memory allocator (dmalloc), highlighting key design decisions, and implementation challenges. The project involved creating a robust memory allocator with statistics tracking, overflow protection, and error detection capabilities.

## Changes Made from the Design Document
1. Metadata Structure
- **Orginial Plan:** Basic implementation header with size and allocation status.
- **Final Design:** 
    - Included source file/line information for debugging
    - Doubly-linked list for allocation tracking
    - Added alignment padding to match test cases.
2. Error Dectection Improvements
- **Original Plan:** Invalid free detection
- **Final Design:** 
    - Added comprehensive checks for mid-block frees, double frees, wild write, etc.
3. Performance Optimization
- **Original Plan:** Simple allocation tracking using map
- **Final Design:** 
    - Linked list for more efficient operations.
    - Improved alignments operations
    - Used bit packing to reduce overhead
4. Statistics Tracking
- **Original Plan:** Basic counters
- **Final Design:** 
    - Added heap boundary tracking
    - Separated counters for failed allocations

## What Worked and What Didn't

### What Worked well:
1. **Canary-Based Corruption Detection**
    - Canaries caught the buffer overflow errors
2. **Distinct Error Reporting**
    - Detailed error messages to correctly identify failure points.
3. **Statistics System**
    - Helped identify memory leaks

### What Needs Improvement:

1. **Initial Alignment Handling**
    - Improper alignment caused crashes in early attempts.
2. **Thread Safety**
    - This implementation doesn't account for multi-threading.
    - Future improvement: add atomic operations.
3. **Performance Under Load**
    - Initial attempts with linked list caused major slowdowns.
4. **test033 Failure**
    - **Current Behavior:** The `dfree()` function fails to identify the second free of `b`:
        - Metadata gets overwritten by `memcpy` operations
        - Canary values are destroyed before the second free
    - **Potential Causes:**
        - Current metadata checks aren't robust enough.
        - No current protection against adjacent block corruption.
        - Boundary checking isn't robust enough.
    - **Future Improvements:**
        1. Better metadata protection
        2. Neighbor block checking
        3. Heap consistency checking

## Research and Challenges

### Key Research Areas:

1. **Memory Allocation Patterns**
- Studied jemalloc and tcmalloc implementations
- Researched slab allocation for potential optimization

2. **Pointer Arithmetic**
- Investigated safe casting techniques
- Researched alignment requirements across architectures

3. **Debugging Techniques**
- Analyzed Valgrind and AddressSanitizer approaches

### Major Challenges:

1. **Wild Write Detection**
- Initial canary implementation had false positives
- Solved by adding padding between allocations

2. **Metadata Corruption**
- Early versions vulnerable to header overwrites
- Added redundant checks and magic numbers

## Time Spent

Total time: ~23hours

Breakdown:
- Design & Initial Research: 6 hours
- Core Program:
    - Initial design: 4 hours
    - Edge cases & test handling: 10 hours
- Extra Credit: 2 hours
- Reflection: 1 hours

## UNIT Test Proposal: Realloc Boundary Test

### Purpose:

Verify drealloc handles various boundary conditions correctly while maintaining memory safety.

### Test Cases:
1. NULL pointer (should dmalloc)
2. Size Expansion
3. Size Reduction
4. Free on Zero (should dfree())

Expected Output:
1. Returns new allocation
2. Returns NULL (after freeing)
3. Aborts with "invalid free" message
4. Aborts with "wild write" detection