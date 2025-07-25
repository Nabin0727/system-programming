# Simple Shell Implementation in C

A basic command-line shell written in C that demonstrates fundamental concepts of process management, input parsing, and system calls in Unix-like operating systems.

## Overview

This project implements a minimal shell that can execute system commands by creating child processes. It showcases core operating system concepts including process creation, program execution, and inter-process communication.

## Features

- Interactive command prompt (`$`)
- Command parsing and execution
- Process management using fork/exec model
- Error handling for system calls
- Graceful exit with Ctrl+D (EOF)

## How It Works

### 1. Main Loop (`main()`)

The shell operates in a continuous read-eval-print loop:

```c
while (1) {
    printf("$ ");                    // Display prompt
    fgets(input, MAX_INPUT_LEN, stdin); // Read user input
    parse_input(input, args);        // Parse into arguments
    execute_command(args);           // Execute the command
}
```

**Key Components:**
- **Prompt Display**: Shows `$` to indicate readiness for input
- **Input Reading**: Uses `fgets()` to safely read up to 1024 characters
- **EOF Handling**: Detects Ctrl+D and exits gracefully
- **Empty Input Skip**: Ignores blank lines

### 2. Input Parsing (`parse_input()`)

Converts the raw input string into an array of arguments suitable for program execution:

```c
void parse_input(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " ");  // Split by spaces
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // execvp requires NULL-terminated array
}
```

**Process:**
1. **Tokenization**: Uses `strtok()` to split input by spaces
2. **Array Population**: Stores each token in the `args` array
3. **Null Termination**: Ensures the array ends with `NULL` (required by `execvp`)
4. **Bounds Checking**: Prevents buffer overflow by limiting to `MAX_ARGS`

**Example:**
- Input: `"ls -la /home"`
- Output: `args = ["ls", "-la", "/home", NULL]`

### 3. Command Execution (`execute_command()`)

Implements the fork-exec model to run external programs:

```c
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
```

**Process Flow:**

#### Fork Operation
- **`fork()`**: Creates an identical copy of the current process
- **Return Values**:
  - `< 0`: Fork failed (error condition)
  - `= 0`: Code is running in child process
  - `> 0`: Code is running in parent process (returns child's PID)

#### Child Process (pid == 0)
- **`execvp(args[0], args)`**: Replaces the child process with the target program
- **Program Search**: `execvp` searches for the program in directories listed in `PATH`
- **Argument Passing**: Passes the parsed arguments to the new program
- **Error Handling**: If `execvp` fails, prints error and exits

#### Parent Process (pid > 0)
- **`waitpid(pid, NULL, 0)`**: Waits for the child process to complete
- **Synchronization**: Ensures the shell waits before showing the next prompt
- **Resource Cleanup**: Prevents zombie processes

## System Calls Used

### Process Management
- **`fork()`**: Creates a new process by duplicating the current one
- **`execvp()`**: Replaces the current process image with a new program
- **`waitpid()`**: Waits for a specific child process to terminate

### Input/Output
- **`fgets()`**: Safely reads a line from stdin with buffer limit
- **`printf()`**: Displays the shell prompt

### String Manipulation
- **`strtok()`**: Tokenizes strings by delimiter
- **`strcspn()`**: Finds the length of initial segment not containing specified characters
- **`strlen()`**: Returns the length of a string

## Compilation and Usage

### Compile
```bash
gcc -o shell shell.c
```

### Run
```bash
./shell
```

### Example Session
```bash
$ ls
file1.txt  file2.txt  shell.c
$ pwd
/home/user/shell-project
$ echo "Hello, World!"
Hello, World!
$ ^D
```

