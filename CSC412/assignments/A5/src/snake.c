#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "game.h"
#include "game_over.h"
#include "game_setup.h"
#include "mbstrings.h"
#include "render.h"

/** Gets the next input from the user, or returns INPUT_NONE if no input is
 * provided quickly enough.
 */
enum input_key get_input() {
    /* DO NOT MODIFY THIS FUNCTION */
    int input = getch();

    if (input == KEY_UP) {
        return INPUT_UP;
    } else if (input == KEY_DOWN) {
        return INPUT_DOWN;
    } else if (input == KEY_LEFT) {
        return INPUT_LEFT;
    } else if (input == KEY_RIGHT) {
        return INPUT_RIGHT;
    } else {
        // if the input isn't an arrow key, we treat it as no input (could add
        // other keys in if we want other commands, like 'pause' or 'quit')
        return INPUT_NONE;
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Helper function that procs the GAME OVER screen and final key prompt.
 * `snake_p` is not needed until Part 2!
 */
void end_game(int* cells, size_t width, size_t height, snake_t* snake_p) {
    // Game over!

    // Free any memory we've taken
    teardown(cells, snake_p);

    // ****************** UNCOMMENT THIS CODE IN PART 2B ***********************
    
    // Render final GAME OVER PRESS ANY KEY TO EXIT screen
    render_game_over(width, height);
    usleep(1000 * 1000);  // 1000ms
    cbreak(); // Leave halfdelay mode
    getch();

    // tell ncurses that we're done
    endwin();
}

int main(int argc, char** argv) {
    // Main program function — this is what gets called when you run the
    // generated executable file from the command line!

    // Board data
    size_t width;   // the width of the board.
    size_t height;  // the height of the board.
    int* cells;     // a pointer to the first integer in an array of integers
                    // representing each board cell.

    // snake data (only used in part 2!)
    snake_t snake;    // your snake struct. (not used until part 2!)
    int snake_grows;  // 1 if snake should grow, 0 otherwise.

    enum board_init_status status;

    // initialize board from command line arguments
    switch (argc) {
        case (2):
            snake_grows = atoi(argv[1]);
            if (snake_grows != 1 && snake_grows != 0) {
                printf(
                    "snake_grows must be either 1 (grows) or 0 (does not "
                    "grow)\n");
                return 0;
            }
            status = initialize_game(&cells, &width, &height, &snake, NULL);
            break;
        case (3):
            snake_grows = atoi(argv[1]);
            if (snake_grows != 1 && snake_grows != 0) {
                printf(
                    "snake_grows must be either 1 (grows) or 0 (does not "
                    "grow)\n");
                return 0;
            } else if (*argv[2] == '\0') {
                status = initialize_game(&cells, &width, &height, &snake, NULL);
                break;
            }
            status = initialize_game(&cells, &width, &height, &snake, argv[2]);
            break;
        case (1):
        default:
            printf("usage: snake <GROWS: 0|1> [BOARD STRING]\n");
            return 0;
    }

    // ----------- DO NOT MODIFY ANYTHING IN `main` ABOVE THIS LINE -----------

    // Check validity of the board before rendering!
    // TODO: Implement (in Part 1C)
    // if ( ? board is not valid ? ) { return EXIT_FAILURE; }
    if (status != INIT_SUCCESS) {
        switch (status) {
            case INIT_ERR_INCORRECT_DIMENSIONS:
                printf("Error: Incorrect board dimensions\n");
                break;
            case INIT_ERR_BAD_CHAR:
                printf("Error: Invalid character in board string\n");
                break;
            case INIT_ERR_WRONG_SNAKE_NUM:
                printf("Error: Board must have exactly one snake\n");
                break;
            default:
                printf("Error: Unknown initialization error\n");
                break;
        }
        return EXIT_FAILURE;
    }

    // Read in the player's name & save its name and length
    char name_buffer[1000];
    read_name(name_buffer);

    g_name_len = mbslen(name_buffer);
    if (g_name_len == (size_t)-1) {
        printf("Error: Invalid UTF-8 in name\n");
        return EXIT_FAILURE;
    }

    // Calculate UTF-8 character length (not byte length)
    g_name_len = mbslen(name_buffer);
    if (g_name_len == (size_t)-1) {
        printf("Error: Invalid UTF-8 in name\n");
        return EXIT_FAILURE;
    }

    // Copy the name safely
    strncpy(g_name, name_buffer, sizeof(g_name) - 1);
    g_name[sizeof(g_name) - 1] = '\0';  // Ensure null-termination

    void read_name(char* write_into);

    initialize_window(width, height);
    while (!g_game_over) {
        usleep(100000);  // 100ms
        enum input_key input = get_input();
        update(cells, width, height, &snake, input, snake_grows);  // Updated to pass snake struct
        render_game(cells, width, height);
    }
    end_game(cells, width, height, &snake);
    return 0;
}
