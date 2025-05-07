# Reflection on A5 - Snake Game Implementation

## Changes Between Design Document and Final Implementation

1. **Snake Movement:** I did not account for special cases in my design document such as preventing a snake of size > 1 from doubling back on itself.
2. **Food placement:** My original design did not include preventing food from appearing on the snake's body, which was necessary during implementation.
3. **Input handling:** The final design included more robust error checking when handling user inputs.
4. ***UTF-8 Character handling:** My design discussed this, but did not go into detail about how to handling 1-4 variable byte lengths in characters.

## What Worked and What Didn’t

### What Worked:
1. **Linked List Integration:** Minimum adjustments were needed to use my previous linked list file for this game.
2. **Collision Detection:** The game correctly ends when the snake encounters external and internal walls or it's own body.
3. **Global Variables:** The game state variables work correctly across the different files.
4. **Scoring:** Food placement and scoring tracker functions correctly as well as snake growth.

### What Didn’t Work:
1. **Testing and Error Prevention:** In order to have a more successful game and account of more errors the code would have to be modified further for handling cases such as the snake occupying the full board.

## Research and Challenges
1. **Direction Change Logic:** The initial implementation did not account for the 180-degree turn and required a separate check to ensure a 180-degree turn could not be done.
2. **Memory Management:** Several issues were encountered due to incorrect memory allocation/deallocation during various game states.
3. **Test Case Development:** Creating useful and unique test cases was difficult and time-consuming. 

### Research:
1. Linked list and memory management techniques.
2. Handling terminal inputs using low-level system calls (read())
3. Run-Length Encoding for the board initialization.
4. Edge scenarios for snake games to use for testing.

## Time Spent
The assignment took ~31 hours to complete, including:
- 5 hours for the initial design, planning, and research.
- 16 hours for the actual coding.
    - 1 hours for initial board and game setup (part 1a)
    - 3 hours for updating and running (part 1b)
    - 5 hours for decompression and flag handling (part 1c)
    - 4 hours for linked list integration (part 2a)
    - 1 hours for game state and game over screen (part 2b)
    - 2 hours to handle UTF-B character sets (part 3a)
- 1 hours for the unicode.md research and writing.
- 7 hours for testing a validation.
- 2 hours for final documentations.

## UNIT Test Proposal
### Purpose:
This test will verify a complete game. Component tests can be found in {A5/test}. The purpose of this UNIT test will be to ensure complete game flow accuracy and verify the integrated functionality of:
1. Snake movement and growth
2. Score counting
3. Food consumption and respawning
4. UTF-8 character handling.
5. Self-collision detection
6. Game over screen rendering.

### Input Data:
1. Initial board setup:
    - 10x10 board with outside walls
    - Snake starts at (2,2) facing right
    - First food item placed on board
2. Player interaction:
    - Name entry: 'ソフィア'
    - Movement commands: 'RRDDLU'
        - movement should include 180-degree turn after eating food and self-collision

### Expected Output:
- Correct board renders given input specifications.
- Prompt appears with `"Name > "`
- Accepts the multibyte character string and correctly interprets the length.
- Snake responds to user inputs
- Snake grows when eating food
- Score increments when food is eaten
- Snake of length > 1 cannot double back
- Self-collision triggers `game_over` state
- `game_over` state displays the name and score

### Explanation:
This single test verfies several key game scenarios to ensure all aspects are behaving as expected.
