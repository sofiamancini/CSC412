# CSC 412: DMalloc
Sofia Mancini
Due: 24th April, 2025
Course: CSC 412 Operating Systems and Networks


## Introduction
This document outlines the design and implementation details for a debugging memory allocator that will aid in catching some bugs often missed when dealing with dynamic memory.

### Planning

### **Part 1 - Statistics**

#### Overview
The goal of this part is to implement memory allocation statistics tracking in a custom memory allocator. We need to maintain accurate counts of various allocation metrics and make them available through a `get_statistics()` function. The statistics will track both successful and failed allocations, heap boundaries, and memory usage patterns.

#### Key Components
1. **Global Statistics Structure:** A single instance of `dmalloc_stats` to accumulate all statistics
2. **Metadata Storage:** A scheme for storing allocation size information with each block
3. **Pointer Adjustment:** Logic to handle the offset between metadata and user-visible pointers
4. **Statistics Updates:** Code to modify statistics during allocation and deallocation

#### Implementation Plan
1. Create a global `dmalloc_stats` variable initialized to zeros
2. Design a metadata header structure to store allocation sizes
3. Modify `dmalloc` to:
    - Allocate extra space for metadata
    - Update statistics for both success and failure cases
    - Track heap boundaries
    - Return pointer adjusted past the metadata
4. Modify `dfree` to:
    - Locate metadata using pointer arithmetic
    - Update active allocation statistics
    - Not modify total/failed statistics (those are allocation-only)

#### Potential Difficulties
1. **Pointer Arithmetic:** Correctly calculating metadata locations without undefined behavior
2. **Alignment:** Ensuring returned pointers meet alignment requirements
3. **Edge Cases:** Handling NULL pointers in `dfree`, extremely large allocations, etc.
4. **Thread Safety:** The global statistics would need protection in multi-threaded code
4. **Metadata Corruption:** Detecting if metadata has been overwritten by buffer overflows

#### Research
1. Study common metadata storage patterns in allocators (header prefixes)
2. Research pointer arithmetic rules and alignment requirements in C++
3. Review memory layout of common allocators like malloc


### **Part 2 - Integer Overflow Protection**

#### Overview
The goal of this part is to implement robust integer overflow protection in memory allocation functions to prevent security vulnerabilities and undefined behavior that could occur when allocation sizes exceed system limits or wrap around due to arithmetic overflow.

#### Key Components
1. **Size Validation:** Checking requested allocation sizes before processing
2. **Safe Arithmetic:** Implementing overflow-checked calculations for:
    - Single allocation sizes
    - Calloc's size × count multiplication
    - Metadata overhead additions
3. **Maximum Allocation Limits:** Defining reasonable upper bounds for allocations
4. **Error Handling:** Properly failing allocations that would overflow

#### Implementation Plan
1. Create overflow-checking helper functions:
    - `check_add_overflow` for size + metadata
    - `check_mul_overflow` for calloc's size × count
2. Modify `dmalloc` to:
    - Validate basic size parameter (reject 0-size allocations)
    - Check for overflow when adding metadata size
    - Verify total allocation doesn't exceed system limits
3. Modify `dcalloc` to:
    - Check for multiplication overflow (size × count)
    - Also apply same checks as dmalloc for final size
4. Update statistics:
    - Count overflow attempts as failures
    - Track failed allocation sizes properly

#### Potential Difficulties
1. **Portability:** Different systems may have varying size_t ranges and behaviors
2. **Performance Impact:** Overflow checks add branches to hot paths
3. **Edge Cases:**
    - SIZE_MAX requests
    - 0-size or 0-count allocations
    - Adding metadata to already-large requests
4. **Testing:** Hard to simulate true overflow conditions in tests
5. **Metadata Calculation:** Ensuring overflow checks include metadata overhead

#### Research
1. Study common integer overflow vulnerabilities in memory allocators
2. Analyze secure implementations in jemalloc/tcmalloc
3. Research compiler intrinsics for overflow detection (_builtin*_overflow)
4. Investigate maximum allocation sizes on modern systems

#### Rough Outline of Functions and Structures

##### Functions

    bool check_add_overflow(size_t a, size_t b, size_t* result);
    bool check_mul_overflow(size_t a, size_t b, size_t* result);

##### Data Structures

    const size_t MAX_ALLOC_SIZE = // Reasonable limit based on system
    (1UL << 30); // e.g., 1GB as practical maximum

    // In dmalloc_stats:
    unsigned long long noverflow; // Count of overflow attempts

### **Part 3 - Invalid Free and Double-free Detection**

#### Overview
This section focuses on implementing robust protection against memory corruption through invalid and double-free operations. The allocator must detect and prevent these common memory errors while providing detailed diagnostic information to help developers identify bugs.

#### Key Components
1. **Heap Boundary Tracking:** Maintain accurate records of allocated memory regions
2. **Allocation State Tracking:** Monitor which blocks are currently allocated vs. freed
3. **Metadata Validation:** Verify integrity of allocation headers
4. **Error Reporting:** Provide detailed diagnostic messages for different failure modes
5. **Pointer Verification:** Validate pointers before processing free operations

#### Implementation Plan
1. Enhance metadata structure to include:
    - Allocation state (allocated/freed)
    - Magic number for header validation
    - Source location information (optional)
2. Implement boundary checking:
    - Verify pointer lies within heap bounds
    - Check alignment requirements
3. Add free operation validation:
    - Detect double-frees
    - Catch invalid pointers
    - Validate metadata integrity
4. Develop error reporting:
    - Standardized error message format
    - Source location tracking (when available)
    - Detailed pointer information

#### Potential Difficulties
1. **Performance Overhead:** Additional checks may impact free operation speed
2. **False Positives:** Potential for incorrectly identifying valid pointers as invalid
3. **Metadata Corruption:** Handling cases where headers have been overwritten
4. **Alignment Challenges:** Properly verifying pointer alignment
5. **Thread Safety:** Potential race conditions in state tracking

#### Research
1. Analyze common memory corruption patterns in C/C++ programs
2. Study existing allocator implementations (jemalloc, tcmalloc, Electric Fence)
3. Review memory debugging tools (Valgrind, AddressSanitizer)
4. Investigate metadata protection techniques (magic numbers, checksums)
5. Examine production crash reporting formats

#### Rough Outline of Functions and Structures

##### Functions

    bool is_in_heap(void* ptr) {
        return ptr >= global_stats.heap_min && 
            ptr <= global_stats.heap_max;
    }

    void report_error(const char* msg, void* ptr) {
        fprintf(stderr, "MEMORY BUG: %s: invalid free of pointer %p\n", 
                current_file_line(), ptr);
    }

##### Data Structures

    struct alloc_metadata {
        size_t size;            // User-requested allocation size
        uint32_t magic;         // Validation number (0xDEADBEEF)
        enum { ALLOCATED, FREED } state;
        #ifdef DEBUG
        const char* file;       // Allocation source file
        int line;               // Allocation line number
        #endif
    };

    const uint32_t MAGIC_NUMBER = 0xDEADBEEF;

### **Part 4 - Boundary Write Error Detection**

#### Overview
This section focuses on detecting buffer overflows and underflows by implementing boundary checking. When memory is freed, the allocator will verify that protective markers around the allocation remain intact, indicating no out-of-bounds writes occurred.

#### Key Components
1. **Canary Values:** Special marker values surrounding allocations
2. **Boundary Verification:** Checking canaries during free operations
3. **Error Reporting:** Detailed diagnostics for boundary violations
4. **Memory Layout:** Strategic placement of canaries around user memory

#### Implementation Plan
1. Modify allocation to:
    - Add leading and trailing canaries around user memory
    - Initialize canaries with distinctive values
2. Enhance free to:
    - Verify canary integrity before deallocation
    - Identify overflow vs underflow violations
    - Report precise error locations
3. Adjust metadata to:
    - Track canary locations
    - Store expected canary values

#### Potential Difficulties
1. **Memory Overhead:** Additional space required for canaries
2. **Alignment Issues:** Maintaining proper alignment with canaries
3. **False Negatives:** Canaries might survive accidental overwrites
4. **Performance Impact:** Additional checks during free operations

#### Rough Outline of Functions and Structures

##### Functions

    bool verify_canaries(alloc_metadata* meta) {
        return memcmp(meta->leading_canary, CANARY_VALUE, CANARY_SIZE) == 0 &&
            memcmp((char*)meta + meta->total_size - CANARY_SIZE, 
                    CANARY_VALUE, CANARY_SIZE) == 0;
    }

##### Data Structures

    const size_t CANARY_SIZE = 8;
    const char CANARY_VALUE[CANARY_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAB, 0xCD};

    struct alloc_metadata {
        char leading_canary[CANARY_SIZE];
        // User memory follows immediately after
        // Trailing canary placed at meta + total_size - CANARY_SIZE
    };

### **Part 5 - Memory Leak Reporting**

#### Overview
The goal is to implement a memory leak detector that tracks all active allocations and reports any blocks that were allocated but not freed before program termination. This helps developers identify missing `dfree()` calls.

#### Key Components
1. **Allocation Tracking:** Maintain a global data structure recording every `dmalloc()` call.
2. **Free Tracking:** Remove entries from the tracker when `dfree()` is called.
3. **Leak Reporting:** Iterate through remaining allocations at program end and print details.
4. **Source Location:** Capture file/line info for debugging (if enabled).

#### Implementation Plan
1. **Global Allocation Map:**
    - Use `std::unordered_map<void*, AllocationInfo>` to track active allocations.
    - Key: User pointer (returned by `dmalloc`).
    - Value: Struct containing size, file, and line number.
2. Enhanced `dmalloc`:
    - Store allocation info in the map when memory is allocated.
3. Enhanced `dfree`:
    - Remove the entry from the map when memory is freed.
4. Leak Report (`print_leak_report`):
    - Iterate through the map and print all remaining allocations.

#### Potential Difficulties
1. **Performance Overhead:**
    - Map operations add latency to `dmalloc/dfree`.
    - Potential Solution: Use an efficient hash map (`std::unordered_map`).
2. **Thread Safety:**
    - If the program is multithreaded, the map needs locking. (Potential improvement)
3. **False Positives:**
    - If `print_leak_report()` is called before intended frees, leaks may be misreported.
4. Pointer Aliasing:
    - If a pointer is modified before being freed, the map lookup could fail.


#### Research

- Existing Tools: Valgrind’s `memcheck`, AddressSanitizer (`-fsanitize=address`).
- Debug Macros: Using `__FILE__` and `__LINE__` for source tracking.

#### Rough Outline of Functions and Structures

##### Functions

    void print_leak_report() {  
        for (const auto& [ptr, info] : allocation_map) {  
            printf("LEAK CHECK: %s:%d: allocated object %p with size %zu\n",  
                info.file ? info.file : "unknown",  
                info.line,  
                ptr,  
                info.size);  
        }  
    }  

##### Data Structures

    #include <unordered_map>  
    std::unordered_map<void*, AllocationInfo> allocation_map;  

    #ifdef DEBUG  
    #define dmalloc(size) dmalloc(size, __FILE__, __LINE__)  
    #endif  

    struct AllocationInfo {  
        size_t size;  
        const char* file;  // Source file (if DEBUG enabled)  
        int line;          // Line number (if DEBUG enabled)  
    };  

### **Part 6 - Advanced Reports and Checking**

#### Overview
This section enhances error reporting for invalid frees by:
1.Detecting "mid-block" frees (pointers inside valid allocations but not at the start).
2. Providing detailed diagnostics, including the nearest valid allocation.

#### Key Components
1. **Pointer Validation:**
    - Check if a freed pointer is exactly at a block start (valid).
    - If not, check if it falls inside another valid block.
2. **Enhanced Error Messages:**
    - Distinguish between:
        - Non-heap pointers (not in heap).
        - Freed pointers (double free).
        - Mid-block pointers (X bytes inside a Y-byte region).
3. **Metadata Integrity Checks:**
    - Verify magic numbers to detect corruption.

#### Implementation Plan
1. Extend `dfree()` Validation Logic:
    - Step 1: Check if pointer is NULL → do nothing.
    - Step 2: Check if pointer is outside heap bounds → not in heap.
    - Step 3: Check if pointer matches a block start → proceed with free.
    - Step 4: If not, search for the nearest enclosing block → mid-block free.
    - Step 5: If no block found → not allocated.
2. Optimize Block Search:
    - Use the allocation map from Part 5 (leak checker) to track active blocks.
    - Iterate through blocks to find if ptr lies within any valid range.
3. Enhanced Error Formatting:
    - Print:
        - Invalid free location (file:line).
        - Nearest allocation (if applicable).

#### Potential Difficulties
1. **Performance Impact:**
    - Searching all blocks for mid-block pointers could be slow for large heaps.
    - Potential Solution: Use an ordered data structure (e.g., `std::map` sorted by address) for faster range queries.
2. **Pointer Alignment:**
    - Misaligned pointers may falsely appear as mid-block.
    - Potential Solution: Reject obviously misaligned pointers early.
3. **Metadata Corruption:**
    - Corrupted headers may cause false negatives.
    - Potential Solution: Magic number checks before trusting metadata.

#### Rough Outline of Functions and Structures

##### Functions

    bool is_in_heap(void* ptr) {  
        return ptr >= global_stats.heap_min && ptr <= global_stats.heap_max;  
    }  

##### Data Structures

    std::unordered_map<void*, AllocationInfo> allocation_map;  

    std::map<void*, AllocationInfo> sorted_allocations; // Sorted by address  

#### Error Messages

1. **Non-Heap Pointer:**

    ```MEMORY BUG: test.c:10: invalid free of 0x123456, not in heap ```

2. **Mid-Block Free:**

    ```MEMORY BUG: test.c:10: invalid free of 0x123456, not allocated  ```
    ```test.c:8: 0x123456 is 16 bytes inside a 64-byte region allocated here  ```

3. **Double Free:**

    ```MEMORY BUG: test.c:10: double free of 0x123456  ```

### **Part 7 - Performance and C++ Integration**

#### Overview
This section focuses on optimizing the debug allocator to:
1. Handle high-frequency allocations (e.g., 500,000+ calls in `test034`).
2. Support C++ allocation patterns (e.g., `new`/`delete`, STL containers).
3. Minimize runtime overhead while maintaining safety checks.

#### Key Components
1. **Optimized Tracking Structures:**
    - Replace `std::unordered_map` with a faster lookup structure for allocations.
2. **C++ Compatibility:**
    - Ensure `dmalloc`/`dfree` work with `operator new`/`delete`.

#### Implementation Plan
1. **Optimize Allocation Tracking:**
    - Option 1: Use a hash table with open addressing (faster than `std::unordered_map`).
    - Option 2: Use a balanced tree (`std::map`) if pointer stability is critical.
    - Option 3: Preallocate metadata to reduce malloc overhead.
2. **C++ Integration:**
    - Override `operator new`/`delete` to route through `dmalloc`/`dfree`:
3. **Performance Testing:**
    - Profile with test034 (500,000 allocations) and optimize bottlenecks.

#### Potential Difficulties
1. **Hash Collisions:**
    - A poor hash function degrades performance.
2. **C++ Edge Cases:**
    - Alignment requirements.
    - Potential Solution: Use `alignof(std::max_align_t)` for allocations.
3. **Debug vs. Release Tradeoffs:**
    - Safety checks add overhead.
    - Potential Solution: Use `#ifdef DEBUG` to disable checks.

#### Rough Outline of Functions and Structures

##### Functions

    // Override global new/delete  
    void* operator new(size_t size) { return dmalloc(size); }  
    void operator delete(void* ptr) noexcept { dfree(ptr); }  

    // Override array new/delete  
    void* operator new[](size_t size) { return dmalloc(size); }  
    void operator delete[](void* ptr) noexcept { dfree(ptr); }  

##### Data Structures

    constexpr size_t kAlignment = alignof(std::max_align_t);  
    void* internal_alloc(size_t size) {  
        return aligned_alloc(kAlignment, size);  
    }  


