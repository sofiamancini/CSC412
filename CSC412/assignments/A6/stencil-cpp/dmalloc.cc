#define DMALLOC_DISABLE 1
#include "dmalloc.hh"
#include <cassert>
#include <cstring>
#include <cstdio>

// Internal metadata struct
struct metadata {
    size_t size;
    const char* file;
    long line;
    bool is_free;
    uintptr_t canary;
    metadata* next; // For linked list tracking
    metadata* prev;

};

static dmalloc_stats global_stats = {0, 0, 0, 0, 0, 0, 0, 0};
static metadata* head = nullptr;
static const uintptr_t CANARY_VALUE = 0xDEADBEEF;

/**
 * dmalloc(sz,file,line)
 *      malloc() wrapper. Dynamically allocate the requested amount `sz` of memory and 
 *      return a pointer to it 
 * 
 * @arg size_t sz : the amount of memory requested 
 * @arg const char *file : a string containing the filename from which dmalloc was called 
 * @arg long line : the line number from which dmalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dmalloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings

    // Check for zero size
    if (sz == 0) {
        return nullptr;
    }

    // Calculate the total size needed
    if (sz > SIZE_MAX - sizeof(metadata)) {
        // Overflow occurred
        global_stats.nfail++;
        global_stats.fail_size += sz;
        return nullptr;
    }
    size_t total_size = sizeof(metadata) + sz + sizeof(uintptr_t);

    // Allocate memory
    metadata* meta = (metadata*)base_malloc(total_size);
    if (meta == nullptr) {
        global_stats.nfail++;
        global_stats.fail_size += sz;
        return nullptr;
    }

    // Initialize metadata
    meta->size = sz;
    meta->file = file;
    meta->line = line;
    meta->is_free = false;
    meta->canary = CANARY_VALUE;
    meta->next = head;
    meta->prev = nullptr;

    // Update linked list
    if (head != nullptr) {
        head->prev = meta;
    }
    head = meta;

    //calculate user pointer
    void* user_ptr = (char*)meta + sizeof(metadata);
    uintptr_t* end_canary = (uintptr_t*)((char*)user_ptr + sz);
    memcpy(end_canary, &CANARY_VALUE, sizeof(uintptr_t));

    global_stats.nactive++;
    global_stats.ntotal++;
    global_stats.active_size += sz;
    global_stats.total_size += sz;

    // Update heap bounds
    uintptr_t addr = (uintptr_t)user_ptr;
    if (global_stats.heap_min == 0 || addr < global_stats.heap_min) {
        global_stats.heap_min = addr;
    }
    if (addr +sz > global_stats.heap_max) {
        global_stats.heap_max = addr + sz;
    }

    return user_ptr;
}

/**
 * dfree(ptr, file, line)
 *      free() wrapper. Release the block of heap memory pointed to by `ptr`. This should 
 *      be a pointer that was previously allocated on the heap. If `ptr` is a nullptr do nothing. 
 * 
 * @arg void *ptr : a pointer to the heap 
 * @arg const char *file : a string containing the filename from which dfree was called 
 * @arg long line : the line number from which dfree was called 
 */
void dfree(void* ptr, const char* file, long line) {
    (void) file, (void) line;

    if (ptr == nullptr) {
        return;
    }

    // Check if pointer is in heap range
    if ((uintptr_t)ptr < global_stats.heap_min || 
        (uintptr_t)ptr > global_stats.heap_max) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not in heap\n",
                file, line, ptr);
        abort();
    }

    // Search metadata list for matching allocation or inside-range bug
    metadata* current = head;
    metadata* found = nullptr;
    while (current != nullptr) {
        void* block_start = (char*)current + sizeof(metadata);
        void* block_end = (char*)block_start + current->size;

        if (ptr == block_start) {
            found = current;
            break;
        }

        if ((uintptr_t)ptr > (uintptr_t)block_start &&
            (uintptr_t)ptr < (uintptr_t)block_end) {
            size_t offset = (uintptr_t)ptr - (uintptr_t)block_start;
            fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n",
                    file, line, ptr);
            fprintf(stderr, "%s:%ld: %p is %zu bytes inside a %zu byte region allocated here\n",
                    current->file, current->line, ptr, offset, current->size);
            abort();
        }

        current = current->next;
    }

    if (found == nullptr) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, double free\n",
                file, line, ptr);
        abort();
    }

    metadata* meta = found;

    // Alignment check
    if ((uintptr_t)ptr % alignof(metadata) != 0) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n",
                file, line, ptr);
        abort();
    }

    // Canary check at start
    if (meta->canary != CANARY_VALUE) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n",
                file, line, ptr);
        abort();
    }

    // Sanity check on is_free
    uint8_t is_free_byte;
    memcpy(&is_free_byte, &meta->is_free, sizeof(uint8_t));
    if (is_free_byte != 0 && is_free_byte != 1) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n",
                file, line, ptr);
        abort();
    }

    // Double free
    if (is_free_byte == 1) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, double free\n",
                file, line, ptr);
        abort();
    }

    // End canary check (wild write detection)
    uintptr_t* end_canary = (uintptr_t*)((char*)ptr + meta->size);
    uintptr_t stored_canary;
    memcpy(&stored_canary, end_canary, sizeof(uintptr_t));
    if (stored_canary != CANARY_VALUE) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: detected wild write during free of pointer %p\n",
                file, line, ptr);
        abort();
    }

    // Mark block as free
    meta->is_free = true;

    // Update stats
    global_stats.nactive--;
    global_stats.active_size -= meta->size;

    // Remove from list
    if (meta->prev) {
        meta->prev->next = meta->next;
    } else {
        head = meta->next;
    }

    if (meta->next) {
        meta->next->prev = meta->prev;
    }

    base_free(meta);
}

/// drealloc(ptr, sz, file, line)
///    Reallocate the dynamic memory pointed to by `ptr` to hold at least
///    `sz` bytes, returning a pointer to the new block. If `ptr` is
///    `nullptr`, behaves like `dmalloc(sz, file, line)`. If `sz` is 0,
///    behaves like `dfree(ptr, file, line)`. The allocation request
///    was at location `file`:`line`.

void* drealloc(void* ptr, size_t sz, const char* file, long line) {
    // If ptr is NULL, equivalent to dmalloc
    if (ptr == nullptr) {
        return dmalloc(sz, file, line);
    }

    // If size is 0, equivalent to dfree
    if (sz == 0) {
        dfree(ptr, file, line);
        return nullptr;
    }

    // Get the metadata for the current allocation
    metadata* meta = (metadata*)((char*)ptr - sizeof(metadata));

    // Check canary values for corruption
    if (meta->canary != CANARY_VALUE) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid realloc of pointer %p, not allocated\n",
                file, line, ptr);
        abort();
    }

    // Check end canary for wild writes
    uintptr_t* end_canary = (uintptr_t*)((char*)ptr + meta->size);
    uintptr_t stored_canary;
    memcpy(&stored_canary, end_canary, sizeof(uintptr_t));
    if (stored_canary != CANARY_VALUE) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: detected wild write during realloc of pointer %p\n",
                file, line, ptr);
        abort();
    }

    // If the requested size is smaller or equal to the current size, we can keep the same block
    if (sz <= meta->size) {
        // Update end canary for the new size
        uintptr_t* new_end_canary = (uintptr_t*)((char*)ptr + sz);
        memcpy(new_end_canary, &CANARY_VALUE, sizeof(uintptr_t));
        
        // Update statistics
        global_stats.active_size -= (meta->size - sz);
        global_stats.total_size -= (meta->size - sz);
        
        meta->size = sz;
        return ptr;
    }

    // Otherwise, we need to allocate a new block and copy the data
    void* new_ptr = dmalloc(sz, file, line);
    if (new_ptr == nullptr) {
        return nullptr;
    }

    // Copy the old data (up to the smaller of the two sizes)
    memcpy(new_ptr, ptr, meta->size);

    // Free the old block
    dfree(ptr, file, line);

    return new_ptr;
}

/**
 * dcalloc(nmemb, sz, file, line)
 *      calloc() wrapper. Dynamically allocate enough memory to store an array of `nmemb` 
 *      number of elements with wach element being `sz` bytes. The memory should be initialized 
 *      to zero  
 * 
 * @arg size_t nmemb : the number of items that space is requested for
 * @arg size_t sz : the size in bytes of the items that space is requested for
 * @arg const char *file : a string containing the filename from which dcalloc was called 
 * @arg long line : the line number from which dcalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dcalloc(size_t nmemb, size_t sz, const char* file, long line) {
    
    // Check for zero size
    if (nmemb == 0 || sz == 0) {
        return nullptr;
    }

    // Calculate the total size needed
    if (sz != 0 && nmemb > SIZE_MAX / sz) {
        // Overflow occurred
        global_stats.nfail++;
        global_stats.fail_size += sz;
        return nullptr;
    }
    size_t total_size = nmemb * sz;

    void* ptr = dmalloc(total_size, file, line);
    if (ptr) {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

/**
 * get_statistics(stats)
 *      fill a dmalloc_stats pointer with the current memory statistics  
 * 
 * @arg dmalloc_stats *stats : a pointer to the the dmalloc_stats struct we want to fill
 */
void get_statistics(dmalloc_stats* stats) {
    *stats = global_stats;
}

/**
 * print_statistics()
 *      print the current memory statistics to stdout       
 */
void print_statistics() {
    dmalloc_stats stats;
    get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

/**
 * print_leaks()
 *      print the current memory leaks to stdout       
 */

void print_leak_report() {
    metadata* current = head;
    while (current != nullptr) {
        if (!current->is_free) {
        fprintf(stdout, "LEAK CHECK: %s:%ld: allocated object %p with size %zu\n",
                current->file, current->line, (char*)current + sizeof(metadata), current->size);
        }
        current = current->next;
    }
}