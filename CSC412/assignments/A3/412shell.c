
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 64
#define MAX_INPUT 1024


typedef struct {
    char* command;
    char* args[MAX_ARGS + 1];
    unsigned short numArgs;
    char* outputFile;
} ParsedCommand;

char* my_strdup(const char* str) {
    char* copy = malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

void free_command(ParsedCommand* command) {
    if (command->command) free(command->command);
    for (int i = 0; i < command->numArgs; i++) {
        if (command->args[i]) free(command->args[i]);
    }
    if (command->outputFile) free(command->outputFile);
}

void do_fork(ParsedCommand* command) {
    pid_t pid = fork();

    if (pid == 0) {
        if (command->outputFile != NULL) {
            int fd = open(command->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd);
        }

        execvp(command->args[0], command->args);
        perror("execvp");
        fprintf(stderr, "Command failed: %s\n", command->command);
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);

        // if (WIFEXITED(status)) {
        //     printf("Process exited with status %d\n", WEXITSTATUS(status));
        // } else {
        //     fprintf(stderr, "Process terminated abnormally\n");
        // }

        free_command(command);
    }
}

ParsedCommand parse_command(char* input) {
    ParsedCommand command = {NULL, {NULL}, 0, NULL};

    char inputCopy[MAX_INPUT];
    snprintf(inputCopy, MAX_INPUT, "%s", input);

    char* saveptr;
    char* token = strtok_r(inputCopy, " ", &saveptr);
    while (token != NULL) {
        if (token[0] == '"' && token[strlen(token) - 1] == '"') {
            token[strlen(token) - 1] = '\0';
            token++;
        }
        if (command.command == NULL) {
            command.command = my_strdup(token);
            if (!command.command) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
        }

        if (strcmp(token, ">") == 0) {
            if (command.command == NULL) {
                fprintf(stderr, "Error: Redirection '>' cannot be the first token\n");
                free_command(&command);
                return command;
            }
            token = strtok_r(NULL, " ", &saveptr);
            if (token) {
                command.outputFile = my_strdup(token);
                if (!command.outputFile) {
                    perror("strdup");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: No output file specified after >\n");
                free_command(&command);
                return command;
            }
        } else {
            command.args[command.numArgs] = my_strdup(token);
            if (!command.args[command.numArgs]) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            command.numArgs++;
        }
        token = strtok_r(NULL, " ", &saveptr);
    }

    if (command.numArgs > 0) {
        command.args[command.numArgs] = NULL;
    }

    return command;
}

int main(void) {
    char input[MAX_INPUT];

    while (printf("412shell> ") && fflush(stdout) == 0 && fgets(input, MAX_INPUT, stdin)) {
        char* newline = strchr(input, '\n');
        if (newline) *newline = '\0';

        if (strlen(input) == 0) {
            continue;
        }

        if (strncmp(input, "exit", 4) == 0) {
            break;
        }

        ParsedCommand command = parse_command(input);
        if (command.command != NULL) {
            do_fork(&command);
        } else {
            free_command(&command);
        }
    }
    return 0;
}