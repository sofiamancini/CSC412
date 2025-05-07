# Lab 5: Snake Check-In + Analogies

Sofia Mancini \
CSC 412 - OS Systems & Networks \
Due: 28th March, 2025

## Part 1: Read A5 & Identify Three Difficult Parts

### 1. Board Decompression Challenges

#### Memory Management Complexity

##### Specific Concerns:
- The shell code's `malloc` for `cells` may not contain the necessary `free` calls in **all** error paths.
    - If an error occurs during decompression, the `malloc` call succeeds but is not paired with a corresponding `free`.

##### What I Need to Research:
- How to handle nested `malloc`/`free` calls when errors occur during parsing.
- How to handle memory cleanup in C with many files and error conditions.

##### Plan:
- Ensure each failure instance in the decompress function is paired with a call to `free`.
- Test with Valgrind to check for memory leaks.

#### Error Handling Logic

#### Specific Concerns:
- The shell code provides several error flags for each potential scenario.
- The provided test suite contains several edge cases and special errors.

##### What I Need to Research:
- Exactly what scenario each of the provided flags is intended to be used for.
- Edge case handling for specific test cases.
    - i.e. two snakes being allocated.

##### Plan:
- Add explicit checks for things such as snake count and correct dimensions. 
- Test each new check for accuracy and for producing the correct error.
- Add significant comments to ensure each check is well understood.

### 2. Growing Snake

#### Linked List List Position Updates

##### Specific Concerns
- Movement Logic:
    - Need to update the snake's linked list such that:
        - The head moves based on user input.
        - Each subsequent link follows the previous link's *old* position.
        - Tail is removed *only if* the snake is not growing (`snake_grows == 0`).
    - While snake length is one, head and tail are the same & growth should be handled separately.

##### What I Need to Research:
- Efficient/best practice traversal patterns for linked lists. (pointer maths, etc.)
- How to re-use the `doublelink` lab code file without casuing additional errors.

##### Plan:
- Practice with small snake movement (length = 1, 2, 3) to understand update patterns.
- Add helper functions to simplify logic
    - `move_head`, `remove_tail`, etc.

#### Preventing Immediate Reversal Logic

##### Specific Concerns:
- Checking for invalid turns
    - Snakes of length > 1 cannot reverse direction.
    - Add checking logic to ignore input and continue in current direction.

##### What I Need to Research:
- How to track the snake’s *previous* direction to validate new inputs.
- Whether to store direction in the `snake_t` struct or derive it from the head-tail segment.

##### Plan:
- Add a `prev_direction` field to `snake_t` and validate:
    ```
    if (snake->length > 1 && new_dir == opposite(prev_dir)) {
        // ignore input, continue in prev_dir
    }
    ```

#### Growth Logic

##### Specific Concerns:
- Conditional Growth:
    - Snake only grows when `snake_grow == 1` *and* it eats food.
    - Ensure there is no `remove_tail` call when snake is growing.

##### What I Need to Research
- How to modify linked list insertion for growth (e.g., always add new head, conditionally remove tail).
- Efficient food collision checks (accessing `cells` array with new head coordinates).

##### Plan:
- Extend `update()` to:
    1. Check `g_game_over` first (early return if true).
    2. Calculate new head position.
    3. If new head hits food and `snake_grows == 1`, increment length *without* removing tail.

### 3. Creating Test Cases

#### Identifying Untested Cases

##### Specific Concerns:
- Exisiting tests are comprehensive and test many basic functions and errors, it will be difficult to identify new or edge cases to be tested. 

##### What I Need to Research:
- Review all existing tests to ensure I am not creating any duplicate or redundant tests.
- Understand game mechanics to identify some edge cases that are currently untested.

##### Plan:
- Define some edge cases:
    - Food exists in last cell when snake occupies the rest of the board.
    - Boards with a single row or column.

#### JSON Syntax

#### Specific Concerns:
- I am largely unfamiliar with JSON so understanding the necessary syntax to write a test.
- Checking formatting
    - Commas, braces, quotes, multi-line inputs, etc.
- Field requirements:
    - including mandatory fields
        - `description`, `seed`, `output`
    - keeping/exlcuding conditional fields
        - `board`, `name`

##### What I Need to Research
- JSON syntax rules.
- How to test runner parses `cells` (array vs string).

##### Plan:
- Create a template for new tests with detailed comments to explain what each line is doing.

#### Ensuring Test Correctness

##### Specific Concerns:
- False Positives/Negatives
    - Ensuring test expected output is aligned with game logic.
- Determinism
    - Random elements *must* be fixed to ensure uniformity.

##### What I Need to Research:
- How to manually verify the tests expectations.
    - Draw the path out on paper.
- Seed behaviour to standardize test.

##### Plan:

- For each new test:
    1. Manually calculate expected output.
    2. Run with the Makefile and debug any discrepancies.
    3. Add additional logging statements to `game.c` for debugging.

## Part 2: Real-World Analogies

### 1. Dynamic Memory Allocation - Formula 1 Pit Stops

There are several aspects of a Formula 1 race pit stop that mirrors the actions of dynamic memory allocation:
- `malloc()` - Prior to the race start, each team is allocated a specific number of resources (tires, fuel, parts) and when coming into the pit, the crew allocates *only* the specific resources needed for that driver. They don't pre-assign every resource for every possible scenario, they react dynamically.
- `free()` - Throughout the race weekend, used tires are discarded (memory is freed), and the remaining resources are returned to the pool for reuse.
- `realloc()` - If conditions change during the race (rain, collisions), the crew might use additional stops. (adjusting memory during runtime)
- **Precision and Speed:** pit stops and dynamic memory allocation must be fast and efficient so there is no wasted time (memory blocks)
- ***Adaptability:** The team cannnot know exactly which resources the car will need before the race. Similarly, the program may not know the memory requirements until runtime.
- **Memory Leaks:** If resources are mismanaged (using all soft tires in FP1), performance for the race will suffer. This can resemble memory leaks when a program fails to free dynamically allocated memory, exhausting the provided resources.
- **Dangling Pointers:** If a team is double-stacking pit stops and accidentally gives the first driver tires meant for the second, both cars may suffer. Like a driver getting the wrong tires leading to uncertain outcomes in the race, freeing memory then referencing that memory can lead to undefined behavior or crashes in a program.

**Source:** https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/

### 2. Test Accuracy - Restaurant Kitchen Quality Control

In a (good) restaurant, each dish on the menu has been rigorously tested and each customer should be receiving exactly the same dish every time they order it.
- A dish should taste the same regardless of who prepared it. Similarly, the outcome of a test should not change depending on who or where the test is being run.
- **False Positives/Negatives:** 
    - A dish may visually appear the same but taste wrong (false positive)
    - A customer may reject a dish even though it is prepared perfectly correctly (false negative)
    - Written down recipes, labeled ingredients, and periodic quality checks can avoid these errors in a kitchen. Similarly, manual calculation of the expected output can avoid these errors in a programming test.
- **Determinism:** 
    - Each customer must receive the same dish, the chefs cannot introduce different seasonings or methods while cooking. (5g of salt vs a pinch)
    - Random elements in a program must be seeded to known values for testing.
- **Manual Verification:** A head chef may periodically taste components to ensure their correctness, and manually drawing the program states to compare to the test can avoid errors.
- **Testing Edge Cases:**
    - Certain specific tests need to be created for edge cases in a program, similarly, in a kitchen situations such as allergies must be handled in a different manner than normal operation.

**Source:** https://sanae.beer/sessions/insp3-from-kitchen-to-code/

- ChatGPT used for revision and clarification.