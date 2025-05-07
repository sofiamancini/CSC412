# CSC 412: Snake Game
Sofia Mancini
Due: 3rd April, 2025
Course: CSC 412 Operating Systems and Networks

## Part 1

### Introduction
This document outlines the design and implementation details for Part 1 of the Snake game, implemented in C. The project involves setting up the game board, initializing the snake, handling user input, and implementing basic game mechanics such as movement, collisions, and food consumption. The purpose of this part is to establish the foundational structure of the game, including the game loop, board representation, and basic snake behavior.

## Planning

### **Part 1a - Board & Game Setup**

#### Overview

The first step is to initialize the game board and set up the game state. This involves:
1. Defining the board dimensions and cell data.
2. Initializing the snake's starting position and direction.
3. Setting up global game variables such as score and game-over status.

#### Key Components
1. Board Data:
    - `width` and `height`: Dimensions of the game board.
    - `cells`: A 2D array of integers representing the state of each cell on the board.
    - Bit flags (`FLAG_PLAIN_CELL`, `FLAG_SNAKE`, `FLAG_WALL`, `FLAG_FOOD`) to represent cell types.
2. Game Data:
    - `g_game_over`: A flag to indicate whether the game is over.
    - `g_score`: The player's score, incremented when the snake eats food.
3. Snake Data:
    - For Part 1, the snake is represented only by its head (length = 1).

#### Implementation Plan
1. Initialize the Board:
    - Call `initialize_default_board` to set up the board dimensions and cell data.
    - Use the `board_init_status` enum to handle initialization errors.
2. Initialize Game State:
    - Set `g_game_over` to 0 (game is running).
    - Set `g_score` to 0.
    - Initialize the snake's starting position and direction (right by default).
3. Game Loop:
    - Implement the core game loop in `snake.c`:
        1. Wait for user input.
        2. Retrieve input.
        3. Update the game state.
        4. Render the new state.

### **Part 1b - Updating and Running the Game**

#### Overview
This part focuses on implementing the game's core mechanics:

1. Moving the snake based on user input.
2. Detecting collisions with walls.
3. Handling food consumption and updating the score.

#### Key Components
1. Snake Movement:
    - The snake moves in one of four directions (up, down, left, right).
    - The snake's new position is calculated based on the current direction.
2. Collision Detection:
    - If the snake collides with a wall, the game ends.
    - Ensure the game ends before the snake visually moves into the wall.
3. Food Handling:
    - If the snake moves onto a cell with food:
        - The food is consumed.
        - The score is incremented.
        - New food is placed on the board using `place_food`.

#### Implementation Plan
1. Update Function:
    - Implement the `update` function in `game.c` to handle:
        - Snake movement.
        - Collision detection.
        - Food consumption.
2. User Input:
    - Modify the game loop in `snake.c` to capture user input and pass it to `update`.
3. Food Placement:
    - Call `place_food` after the snake eats food to spawn new food.

### **Part 1c - Board Decompression**

#### Overview
The board is represented using **Run-Length Encoding (RLE)**, a compression technique. This part involves decompressing the RLE string to initialize the board.

#### Key Components
1. RLE Format:
    - Example: `B7x10|W10|W1E4W5|...`
    - Letters represent cell types (`B` for blank, `W` for wall, etc.).
    - Numbers represent the count of consecutive cells.
2. Error Handling:
    - Handle errors such as incorrect dimensions, wrong snake count, or unexpected characters.

#### Implementation Plan
1. Decompress RLE String:
    - Parse the RLE string to populate the `cells` array.
    - Ensure the dimensions match the specified width and height.
2. Error Checking:
    Validate the RLE string for errors using the `board_init_status` enum.

### Potential Difficulties
1. Board Initialization:
    - Ensuring the board is correctly initialized with the right dimensions and cell data.
    - Handling errors in the RLE string.
2. Snake Movement:
    - Calculating the snake's new position without going out of bounds.
    - Detecting collisions accurately.
    - Ending game loop at correct times.
3. User Input Handling:
    - Ensuring smooth and responsive input handling.
    - Preventing the snake from moving in the opposite direction (e.g., left while moving right).
4. Food Placement:
    - Ensuring food is placed on valid cells (not on walls or the snake).

### Research
1. Run-Length Encoding:
    - Study RLE compression and decompression techniques.
    - Understand how to parse and validate RLE strings.

2. Collision Detection:
    - Research algorithms for detecting collisions in grid-based games.
3. Game Loop Design:
    - Study best practices for implementing game loops in C.

### Rough Outline of Functions and Structures

#### Functions

1. Board Setup:
    - `initialize_default_board(int** cells_p, size_t* width_p, size_t* height_p)`: Initializes the board.
    - `initialize_game(int** cells_p, size_t* width_p, size_t* height_p, snake_t* snake_p)`: Sets up the game state.

2. Game Loop:
    - `main()` in `snake.c`: Implements the core game loop.

3. Update Logic:
    - `update(input_key key)` in `game.c`: Handles snake movement, collisions, and food consumption.

4. RLE Decompression:
    - Functions in `game_setup.c` to parse and validate the RLE string.

#### Data Structures

1. Board:
    - `int* cells`: Array representing the board.
    - `size_t width`, `size_t height`: Board dimensions.

2. Snake:
    - `snake_t`: Represents the snake's position and direction.

3. Game State:
    - `int g_game_over`: Game-over flag.
    - `int g_score`: Player score.

This descriptions provides a roadmap for implementing Part 1 of the Snake game. By following this plan, the foundational structure of the game will be established, paving the way for more advanced features in Part 2.

## Part 2

### Introduction
This document outlines the design and implementation details for Part 2 of the Snake game, implemented in C. The project focuses on enhancing the snake's functionality by implementing growth when it eats food, improving the data structure for the snake using linked lists, and adding a personalized Game Over screen. The purpose of this part is to extend the game's functionality, making it more dynamic and user-friendly.

## Planning

### **Part 2a - Growing Snake**

#### Overview

In Part 1, the snake was represented only by its head. In Part 2, the snake will grow in length when it eats food. This requires:
1. Redesigning the snake's data structure to support multiple segments.
2. Implementing a linked list to manage the snake's body segments.
3. Updating the game logic to handle snake growth and movement.

#### Key Components
1. Snake Data Structure:
    - Replace global variables with a `snake_t` struct to store the snake's data.
    - Use a linked list to represent the snake's body segments.
2. Snake Growth:
    - When the snake eats food, add a new segment to the linked list.
    - Ensure the snake grows only when `snake_grows` is set to 1.
3. Movement Logic:
    - Update the snake's position by moving the head and adjusting the tail.
    - Handle edge cases, such as the snake colliding with itself or reversing direction.

#### Implementation Plan
1. Update Snake Struct:
    - Modify the `snake_t` struct in `common.h` to include a linked list for the snake's body.
    - Remove global variables for snake data and use the struct instead.
2. Linked List Implementation:
    - Use the linked list implementation from Lab DoubleLink to manage the snake's body segments.
    - Add functions to insert, remove, and update nodes in the linked list.
3. Snake Growth:
    - Modify the `update` function in `game.c` to handle snake growth when food is eaten.
    - Ensure the snake grows only when `snake_grows` is 1.
4. Movement Logic:
    - Update the snake's position by moving the head and adjusting the tail.
    - Handle edge cases, such as the snake colliding with itself or reversing direction.

### **Part 2b - Game Over**

#### Overview
This part focuses on implementing a personalized Game Over screen that displays the player's name and score. This involves:
1. Adding a prompt for the player to enter their name.
2. Rendering the Game Over screen with the player's name and score.
3. Ensuring the game loop terminates correctly when the game is over.

#### Key Components
1. Game Over Screen:
    - Use the provided `render_game_over` function to display the Game Over screen.
    - Include the player's name and score in the display.
2. User Input:
    - Implement the `read_name` function in `game.c` to prompt the player for their name.
    - Use the `read` system call to capture user input.
3. Game Loop Termination:
    - Ensure the game loop terminates correctly when the game is over.
    - Prevent further updates to the game state after the game ends.

#### Implementation Plan
1. Add Game Over Screen:
    - Add `game_over.o` to the `OBJS` list in the Makefile to include the Game Over screen functionality.
    - Ensure the necessary global variables for the player's name are defined in `common.h` and `common.c`.
2. Implement `read_name` Function:
    - Prompt the player to enter their name using `printf`.
    - Use the `read` system call to capture input from standard input (file descriptor 0).
    - Handle edge cases, such as empty input, by reprompting the player.
3. Render Game Over Screen:
    - Call `render_game_over` in the game loop when `g_game_over` is set to 1.
    - Pass the player's name and score to the `render_game_over` function.
4. Terminate Game Loop:
    - Ensure the game loop breaks when `g_game_over` is set to 1.
    - Prevent further updates to the game state after the game ends.

### Potential Difficulties

1. Linked List Management:
    - Ensuring the linked list is correctly updated when the snake moves or grows.
    - Handling edge cases, such as the snake colliding with itself or reversing direction.
2. User Input Handling:
    - Capturing and validating user input for the player's name.
    - Ensuring the input is correctly stored and displayed on the Game Over screen.
3. Game Over Screen Rendering:
    - Ensuring the Game Over screen is displayed correctly with the player's name and score.
    - Handling cases where the player's name exceeds the buffer size.

### Research
1. Linked Lists:
    - Review the linked list implementation from Lab DoubleLink.
    - Study best practices for managing linked lists in C.
2. User Input Handling:
    - Research the `read` system call and file descriptors in C.
    - Study techniques for validating and processing user input.
3. Game Over Screen Design:
    - Explore best practices for rendering text-based screens in terminal applications.

### Outline of Functions and Data Structures

1. Snake Movement and Growth:
    - `update(input_key key)` in `game.c`: Handles snake movement, growth, and collisions.
    - `insert_node`, `remove_node`, `update_node`: Functions to manage the linked list.
2. Game Over Screen:
    - `read_name(char* write_into)` in `game.c`: Prompts the player for their name and captures input.
    -  `render_game_over()` in `game_over.c`: Renders the Game Over screen with the player's name and score.
3. Game Loop:
    - `main()` in `snake.c`: Implements the core game loop and terminates when the game is over.

#### Data Structures

1. Snake:
    - `snake_t`: Struct to store the snake's data, including a linked list for the body segments.

2. Linked List:
    - `node_t`: Struct to represent a node in the linked list.
    - `linked_list_t`: Struct to manage the linked list (optional, depending on implementation).
3. Game State:
    - `int g_game_over`: Game-over flag.
    - `int g_score`: Player score.
    - `char* player_name`: Buffer to store the player's name.

## Part 3

### Introduction
This document outlines the design and implementation details for Part 3 of the Snake game, implemented in C. The focus of this part is on Socially Responsible Computing, specifically ensuring that the game supports multibyte character encoding (UTF-8) to handle player names in various languages and scripts. This part also ties together the final components of the game, including rendering the Game Over screen with the player's name and score. The purpose of this part is to make the game more inclusive and accessible to players from diverse linguistic backgrounds.

## Planning

### **Part 3a - Character Sets**

#### Overview
The default string encoding in C, ASCII, is limited to English and does not support characters from non-Latin alphabets or special symbols (e.g., emojis). To ensure the game can handle player names in any language or script, we will implement support for UTF-8, a widely used multibyte character encoding. This involves:
    1. Understanding UTF-8 encoding and its variable-length character representation.
    2. Implementing a function to count the number of UTF-8 code points (characters) in a string.
    3. Integrating this functionality into the game to handle player names correctly.

#### Key Components
1. UTF-8 Encoding:
    - UTF-8 characters can be 1 to 4 bytes long.
    -   The first byte indicates the length of the character.
2. Multibyte String Handling:
    - Implement the `mbslen` function to count UTF-8 code points in a string.
    - Ensure the function correctly handles variable-length characters and stops at the null terminator.
3. Integration with Game:
    - Use `mbslen` to calculate the length of the player's name in UTF-8 characters.
    - Store the name and its length for rendering on the Game Over screen.

#### Implementation Plan
1. Implement `mbslen` Function:
    - Write the `mbslen` function in `mbstrings.c`.
    - Iterate through the input string, identifying the start of each UTF-8 character using the encoding rules.
    - Count the number of valid UTF-8 code points until the null terminator is reached.
2. Integrate `mbslen` into the Game:
    - Call `mbslen` in `snake.c` after reading the player's name.
    - Store the result in the appropriate variable for use in the Game Over screen.
3. Render Game Over Screen:
    - Uncomment and modify the `end_game` function to call `render_game_over` with the player's name and its UTF-8 length.
    - Ensure the Game Over screen displays the name and score correctly.

### Potential Difficulties
1. UTF-8 Encoding:
    - Understanding the variable-length encoding scheme and correctly identifying the start of each character.
    - Handling edge cases, such as invalid UTF-8 sequences or incomplete characters.
2. Integration with Existing Code:
    - Ensuring the `mbslen` function works seamlessly with the existing game logic.
    - Properly storing and passing the player's name and its length to the Game Over screen.
3. Testing:
    - Testing the game with names in various languages and scripts to ensure compatibility.
    - Verifying that the Game Over screen renders correctly for all valid inputs.

### Research
1. UTF-8 Encoding:
    - Study the UTF-8 encoding standard and its byte-level representation.
    - Review examples of multibyte character handling in C.
2. Multibyte String Functions:
    - Research standard library functions for handling multibyte strings (e.g., mblen, mbstowcs).
    - Understand their limitations and how to implement custom solutions.

### Outline of Functions and Data Structures

#### Functions
1. Multibyte String Handling:
    - `mbslen(const char *bytes)` in `mbstrings.c`: Counts the number of UTF-8 code points in a string.
2. Game Over Screen:
    - `end_game(...)` in `game_over.c`: Calls render_game_over to display the final screen.
    - `render_game_over(...)` in `game_over.c`: Renders the Game Over screen with the player's name and score.
3. Game Loop:
    - `main()` in `snake.c`: Integrates `mbslen` and ensures the Game Over screen is displayed correctly.

#### Data Structures
1. Player Name:
    - `char* player_name`: Buffer to store the player's name.
    - `size_t name_length`: Stores the length of the player's name in UTF-8 code points.
2. Game State:
    - `int g_game_over`: Game-over flag.
    - `int g_score`: Player score.

## Part 4

### Introduction
This document outlines the design and implementation details for Part 4 of the Snake game, implemented in C. The focus of this part is on writing custom tests to ensure the game functions correctly and handles edge cases. The purpose of this part is to ensure the game is robust, reliable, and free of bugs.

## Planning

### **Part 4 - Write Your Own Tests**

#### Overview
In this part, we will write three new tests to validate different aspects of the game. These tests will be added to `tests/traces_new.json` and can be executed using the command `make check-new`. The goal is to test various features of the game, including edge cases, to ensure the game behaves as expected under different conditions.

#### Key Components

1. Test Design:
    - Identify key features and edge cases to test.
    - Ensure the tests cover different parts of the game, such as:
        - Snake movement and growth.
        - Collision detection (walls, self-collision).
        - Food placement and consumption.
        - Game Over screen rendering.
        - Multibyte character handling (UTF-8 names).

2. Test Implementation:
    - Write test cases in `tests/traces_new.json`.
    - Use the existing tests in `tests/traces.json` as a reference for formatting and structure.

3. Test Execution:
    - Run the tests using `make check-new`.
    - Analyze the results and debug any failures.

#### Implementation Plan

1. Identify Test Scenarios:
    - Examples:
        - Snake grows correctly after eating food.
        - Snake dies when colliding with a wall or itself.
        - Game Over screen displays the correct name and score.
        - UTF-8 names with multibyte characters are handled correctly.

2. Write Test Cases:
    - Add three new test cases to `tests/traces_new.json`.
    - Ensure the tests are unique and not duplicates of existing tests in `tests/traces.json`.

3. Run and Validate Tests:
    - Execute the tests using `make check-new`.
    - Verify that all tests pass. If any fail, debug and fix the issues.

### Potential Difficulties

1. Test Design:
    - Identifying meaningful test cases that cover a wide range of functionality.
    - Ensuring edge cases are adequately tested.

2. Test Implementation:
    - Writing test cases in the correct format for `traces_new.json`.
    - Ensuring the tests are independent and do not interfere with each other.

3. Debugging Failures:
    - Diagnosing and fixing issues when tests fail.
    - Ensuring fixes do not introduce new bugs.

### Research
1. Testing Best Practices:
    - Study principles of software testing, such as unit testing, integration testing, and edge case testing.
    - Learn about test-driven development (TDD) and its benefits.

2. JSON Test Format:
    - Review the structure of `traces.json` to understand how tests are formatted.
    - Learn about JSON syntax and how to write valid JSON files.

### Rough Outline of Test Cases

#### Test Case 1: Snake Growth After Eating Food
- Description: Verify that the snake grows by one segment after eating food.
- Steps:
    1. Place food adjacent to the snake's head.
    2. Move the snake to eat the food.
    3. Check that the snake's length increases by one.
- **Expected Result**: Snake length increases, and new food is placed on the board.

#### Test Case 2: Collision with Wall
- Description: Verify that the game ends when the snake collides with a wall.
- Steps:
    1. Move the snake toward a wall.
    2. Check that the game ends when the snake hits the wall.
- Expected Result: `g_game_over` is set to 1, and the game loop terminates.

#### Test Case 3: UTF-8 Name Handling
- Description: Verify that the game correctly handles and displays a player name with multibyte UTF-8 characters.
- Steps:
    1. Enter a name with multibyte characters (e.g., "γνωρίζω" or "宇助").
    2. Check that the name is stored and displayed correctly on the Game Over screen.
- Expected Result: The name and its UTF-8 length are correctly rendered on the Game Over screen.