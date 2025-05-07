#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>  

#include "linked_list.h"
#include "mbstrings.h"

void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    if (g_game_over || !snake_p || !snake_p->head) {
        return;
    }

    // Store old head position
    snake_segment_t* head_seg = (snake_segment_t*)get_first(snake_p->head);
    if (!head_seg) {
        return;
    }
    int old_x = head_seg->x;
    int old_y = head_seg->y;

    // Calculate new head position
    int new_x = old_x;
    int new_y = old_y;
    enum input_key new_dir = snake_p->direction;

    // Handle input direction changes
    if (input != INPUT_NONE) {
        bool is_valid = true;
        
        if (snake_p->head->next != NULL) {
            if ((input == INPUT_UP && snake_p->direction == INPUT_DOWN) ||
                (input == INPUT_DOWN && snake_p->direction == INPUT_UP) ||
                (input == INPUT_LEFT && snake_p->direction == INPUT_RIGHT) ||
                (input == INPUT_RIGHT && snake_p->direction == INPUT_LEFT)) {
                is_valid = false;
            }
        }
        
        if (is_valid) {
            new_dir = input;
        }
    }

    switch(new_dir) {
        case INPUT_UP:    new_y--; break;
        case INPUT_DOWN:  new_y++; break;
        case INPUT_LEFT:  new_x--; break;
        case INPUT_RIGHT: new_x++; break;
        case INPUT_NONE:  return;
    }

    // Check for collisions first
    if (new_x < 0 || new_y < 0 || new_x >= (int)width || new_y >= (int)height ||
        (cells[new_y * width + new_x] & FLAG_WALL)) {
        g_game_over = 1;
        return;
    }

    // Check for food before modifying snake
    int ate_food = (cells[new_y * width + new_x] & FLAG_FOOD);
    
    // Remove tail first (unless growing)
    if (!ate_food || !growing) {
        void* old_tail = remove_last(&snake_p->head);
        if (old_tail) {
            free(old_tail);
        }
    }

    // Check for self-collision after removing tail
    node_t* current = snake_p->head;
    while (current) {
        snake_segment_t* seg = (snake_segment_t*)current->data;
        if (seg->x == new_x && seg->y == new_y) {
            g_game_over = 1;
            return;
        }
        current = current->next;
    }

    // Create new head segment
    snake_segment_t new_head = {
        .x = new_x,
        .y = new_y,
        .direction = new_dir
    };
    
    // Insert new head
    if (!insert_first(&snake_p->head, &new_head, sizeof(snake_segment_t))) {
        g_game_over = 1;
        return;
    }
    snake_p->direction = new_dir;

    if (ate_food) {
        g_score++;
        // Clear the food cell
        cells[new_y * width + new_x] = FLAG_PLAIN_CELL;
        
        // Place new food with verification
        bool food_placed = false;
        while (!food_placed) {
            place_food(cells, width, height);
            food_placed = true;
            
            // Verify food isn't on snake
            node_t* check = snake_p->head;
            while (check) {
                snake_segment_t* seg = (snake_segment_t*)check->data;
                if (cells[seg->y * width + seg->x] == FLAG_FOOD) {
                    cells[seg->y * width + seg->x] = FLAG_PLAIN_CELL;
                    food_placed = false;
                    break;
                }
                check = check->next;
            }
        }
    }

    // Clear all snake cells first
    for (size_t i = 0; i < width * height; i++) {
        if (cells[i] == FLAG_SNAKE) {
            cells[i] = FLAG_PLAIN_CELL;
        }
    }

    // Mark all snake segments on board
    current = snake_p->head;
    while (current) {
        snake_segment_t* seg = (snake_segment_t*)current->data;
        cells[seg->y * width + seg->x] = FLAG_SNAKE;
        current = current->next;
    }
}

void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells + food_index) == FLAG_PLAIN_CELL) {
        *(cells + food_index) = FLAG_FOOD;
    } else {
        place_food(cells, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

void read_name(char* write_into) {
    int attempts = 0;
    
    while (1) {
        printf("Name > ");
        fflush(stdout);

        ssize_t bytes_read = read(STDIN_FILENO, write_into, 999);
        
        if (bytes_read == -1) {
            perror("Error reading input");
            continue;
        }

        write_into[bytes_read] = '\0';
        if (bytes_read > 0 && write_into[bytes_read-1] == '\n') {
            write_into[--bytes_read] = '\0';
        }

        // Handle empty input
        if (bytes_read <= 0) {
            printf("Name Invalid: must be longer than 0 characters.\n");
            if (++attempts > 3) {
                strcpy(write_into, "ValidName");
                return;
            }
            continue;
        }

        // Handle long input
        if (bytes_read == 999) {
            char extra;
            if (read(STDIN_FILENO, &extra, 1) > 0) {
                printf("Name Invalid: must be less than 1000 characters.\n");
                while (read(STDIN_FILENO, &extra, 1) > 0 && extra != '\n');
                if (++attempts > 3) {
                    strcpy(write_into, "ValidName");
                    return;
                }
                continue;
            }
        }

        break; // Valid input received
    }
}

void teardown(int* cells, snake_t* snake_p) {
    if (cells) free(cells);
    
    if (snake_p) {
        while (snake_p->head) {
            node_t* to_remove = snake_p->head;
            snake_p->head = to_remove->next;
            free(to_remove->data);
            free(to_remove);
        }
    }
}
