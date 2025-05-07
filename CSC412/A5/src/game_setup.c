#include "game_setup.h"
#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                     size_t* height_p, snake_t* snake_p,
                                     char* board_rep) {
    enum board_init_status status;
    
    if (snake_p) {
        snake_p->head = NULL;
        snake_p->direction = INPUT_RIGHT;
        snake_p->length = 0;
    }

    if (board_rep == NULL) {
        status = initialize_default_board(cells_p, width_p, height_p);
        if (status == INIT_SUCCESS) {
            // Create temporary stack-allocated segment
            snake_segment_t temp_segment = {
                .x = 2,
                .y = 2,
                .direction = INPUT_RIGHT
            };

            // Let insert_first create the heap copy
            if (!insert_first(&snake_p->head, &temp_segment, sizeof(snake_segment_t))) {
                free(*cells_p);
                *cells_p = NULL;
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }

            snake_p->direction = INPUT_RIGHT;
            snake_p->length = 1;
            place_food(*cells_p, *width_p, *height_p);
        }
    } else {
        status = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    }

    g_game_over = 0;
    g_score = 0;
    return status;
}
                                       

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */
enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed) {

    *cells_p = NULL;

    // Check if the compressed string is in the correct format
    if (!compressed || compressed[0] != 'B') {
        return INIT_ERR_BAD_CHAR;
    }

    // Check if the compressed string is in the correct format
    char* x_position = strchr(compressed, 'x');
    char* pipe_position = strchr(compressed, '|');

    // validate dimensions
    if (!x_position || !pipe_position || x_position > pipe_position) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    *height_p = atoi(compressed + 1); // +1 to skip the 'B'
    *width_p = atoi(x_position + 1); // +1 to skip the 'x'

    if (*width_p <= 0 || *height_p <= 0) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    // Allocate memory for the cells array
    int* cells = malloc(*width_p * *height_p * sizeof(int));
    if (!cells) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    // Initialize the cells array as plain cells
    for (size_t i = 0; i < *width_p * *height_p; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Parse rows
    char* row_str = pipe_position + 1;
    int snake_count = 0;
    size_t row = 0;
    size_t actual_rows = 0;

    while (row_str) {
        size_t col = 0;
        char* next_row = strchr(row_str, '|');

        actual_rows++;

        if (actual_rows > *height_p) {
            free(cells);
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }

        while (*row_str && row_str != next_row) {
            if (!isalpha(*row_str)) {
                free(cells);
                return INIT_ERR_BAD_CHAR;
            }

            char cell_type = *row_str++;
            int cell_count = 0;

            while (isdigit(*row_str)) {
                cell_count = cell_count * 10 + (*row_str++ - '0');
            }

            if (cell_count <= 0) {
                free(cells);
                return INIT_ERR_BAD_CHAR;
            }

            int flag;
            switch (toupper(cell_type)) {
                case 'E': flag = FLAG_PLAIN_CELL; break;
                case 'W': flag = FLAG_WALL; break;
                case 'S':
                    flag = FLAG_SNAKE;
                    snake_count += cell_count;
                    if (snake_count > 1) {
                        free(cells);
                        return INIT_ERR_WRONG_SNAKE_NUM;
                    }
                    // Create temporary stack-allocated segment
                    snake_segment_t temp_segment = {
                        .x = col,
                        .y = row,
                        .direction = INPUT_RIGHT
                    };

                    snake_p->head = NULL;
                    if (!insert_first(&snake_p->head, &temp_segment, sizeof(snake_segment_t))) {
                        free(cells);
                        return INIT_ERR_INCORRECT_DIMENSIONS;
                    }
                    snake_p->direction = INPUT_RIGHT;
                    snake_p->length = 1;
                    break;
                default:
                    free(cells);
                    return INIT_ERR_BAD_CHAR;
            }
            // fill in the cells
            if (col + cell_count > *width_p) {
                free(cells);
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }
            for (int i = 0; i < cell_count; i++) {
                cells[row * *width_p + col++] = flag;
            }
        }
        if (col != *width_p) {
            free(cells);
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }
        row++;
        row_str = next_row ? next_row + 1 : NULL;
    }

    if (actual_rows != *height_p) {
        free(cells);
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    // validate snake count and dimensions
    if (row != *height_p) {
        free(cells);
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }
    if (snake_count != 1) {
        free(cells);
        return INIT_ERR_WRONG_SNAKE_NUM;
    }

    *cells_p = cells;
    place_food(cells, *width_p, *height_p);
    return INIT_SUCCESS;
}
