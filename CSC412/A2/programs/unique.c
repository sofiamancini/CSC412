#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // creates a 1024 byte buffer to store data, will this be enough memory to store data?
    char buffer[1024];
    char last_word[1024];

    // Read data from stdin and write it to stdout (standard output)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // remove newline character

        if (strcmp(buffer, last_word) != 0) {
            puts(buffer);
            strcpy(last_word, buffer);
        }
    }

    return 0;
}