# writing your own shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LEN 1024
#define MAX_ARGS 64

void parse_input(char *input, char **args);
void execute_command(char **args);

int main() {
    char input[MAX_INPUT_LEN];
    char *args[MAX_ARGS];

    while (1) {
        // Print prompt and read input
        printf("$ ");
        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
            printf("\n");  // Handle Ctrl+D (EOF)
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Skip empty input
        if (strlen(input) == 0) {
            continue;
        }

        // Parse and execute
        parse_input(input, args);
        execute_command(args);
    }

    return 0;
}
void parse_input(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " ");  // Split by spaces

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // execvp requires NULL-terminated array
}
void execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {  // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
    else {  // Parent process
        waitpid(pid, NULL, 0);  // Wait for child to finish
    }
}
