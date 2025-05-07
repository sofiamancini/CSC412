#include "dmalloc.hh"
#include <cstdio>
#include <cassert>
#include <cstring>

int main() {
    // Initial statistics should be zero
    print_statistics();
    printf("\n");

    // Test 1: realloc NULL should behave like malloc
    int* ptr1 = (int*)drealloc(nullptr, sizeof(int), __FILE__, __LINE__);
    assert(ptr1 != nullptr);
    *ptr1 = 42;
    printf("Test 1 passed: realloc NULL worked like malloc\n");
    print_statistics();
    printf("\n");

    // Test 2: realloc to larger size
    int* ptr2 = (int*)drealloc(ptr1, 2 * sizeof(int), __FILE__, __LINE__);
    assert(ptr2 != nullptr);
    assert(ptr2 != ptr1); // Should get new memory (implementation dependent)
    ptr2[0] = 42; // Preserve old value
    ptr2[1] = 43; // New value
    printf("Test 2 passed: realloc to larger size\n");
    print_statistics();
    printf("\n");

    // Test 3: realloc to smaller size
    int* ptr3 = (int*)drealloc(ptr2, sizeof(int), __FILE__, __LINE__);
    assert(ptr3 != nullptr);
    assert(*ptr3 == 42); // Should preserve first value
    printf("Test 3 passed: realloc to smaller size\n");
    print_statistics();
    printf("\n");

    // Test 4: realloc to size 0 should behave like free
    void* ptr4 = drealloc(ptr3, 0, __FILE__, __LINE__);
    assert(ptr4 == nullptr);
    printf("Test 4 passed: realloc to size 0 worked like free\n");
    print_statistics();
    printf("\n");

    return 0;
}

// Expected output (except for the corruption test which should abort):
//! alloc count: active          0   total          0   fail          0
//! alloc size:  active          0   total          0   fail          0
//! 
//! Test 1 passed: realloc NULL worked like malloc
//! alloc count: active          1   total          1   fail          0
//! alloc size:  active          4   total          4   fail          0
//! 
//! Test 2 passed: realloc to larger size
//! alloc count: active          1   total          2   fail          0
//! alloc size:  active          8   total         12   fail          0
//! 
//! Test 3 passed: realloc to smaller size
//! alloc count: active          1   total          3   fail          0
//! alloc size:  active          4   total         16   fail          0
//! 
//! Test 4 passed: realloc to size 0 worked like free
//! alloc count: active          0   total          3   fail          0
//! alloc size:  active          0   total         16   fail          0