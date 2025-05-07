#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int toLower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

int main() {
    // creates a 1024 byte buffer to store data, will this be enough memory to store data?
    char buffer[1024];

    // Read data from stdin and write it to stdout (standard output)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // pass data from the buffer to a function to process the data
        for (size_t i = 0; i < strlen(buffer); i++) {
            buffer[i] = toLower(buffer[i]);
        }
        fputs(buffer, stdout);
    }

    return 0;
}