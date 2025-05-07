#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define OUTPUT_FILE "fsrecursion_output.text"

// Recursive function to traverse the file system
void recurve_process(FILE *output, const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return; // Skip inaccessible directories
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip current and parent directories
        }

        // Construct full path dynamically
        char *new_path;
        if (asprintf(&new_path, "%s/%s", path, entry->d_name) == -1) {
            perror("Error allocating memory for new path");
            continue; // Memory allocation failed, skip
        }

        // Check if it's a directory
        struct stat path_stat;
        if (stat(new_path, &path_stat) == -1) {
            perror("Error retrieving file status");
            free(new_path);
            continue;
        }

        if (S_ISDIR(path_stat.st_mode)) {
            fprintf(output, "%s\n", entry->d_name);
            printf("%s\n", entry->d_name);
            fflush(output); 

            recurve_process(output, new_path); // Recurse into subdirectory
        }

        free(new_path); // Free allocated memory
    }

    closedir(dir);
}

int main() {
    char start_path[1024];

    // Read starting directory from file
    FILE *input = fopen("fsrecursion_start.text", "r");
    if (!input) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    if (!fgets(start_path, sizeof(start_path), input)) {
        perror("Error reading start directory");
        fclose(input);
        return EXIT_FAILURE;
    }
    fclose(input);

    // Remove newline character if present
    start_path[strcspn(start_path, "\n")] = '\0';
    printf("Start path: '%s'\n", start_path); // Debugging output

    // Open output file
    FILE *output = fopen(OUTPUT_FILE, "w");
    if (!output) {
        perror("Error opening output file");
        return EXIT_FAILURE;
    }

    // Start recursive traversal
    recurve_process(output, start_path);

    fclose(output);
    return EXIT_SUCCESS;
}
