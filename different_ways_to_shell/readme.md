# Custom C Shell – Minimal Shell Interface in C

Welcome to a simple custom shell implemented in C. This project is designed to provide a foundational understanding of how command-line interpreters work under the hood. It replicates basic shell behavior: reading user input, tokenizing commands, handling built-ins, and executing external programs using `fork()` and `execvp()`.

## 🚀 Features

- Reads and parses user input using `getline()` and `strtok()`
- Displays current working directory in prompt
- Executes external commands using `execvp()`
- Handles process creation with `fork()` and `waitpid()`
- Cleanly exits on `EOF` (Ctrl+D)
- Displays a custom ASCII banner on launch

## 📁 File Structure

- `shell.c` – Core shell logic (input, parsing, execution)
- `banner.c` – ASCII banner function for splash screen
- `main.c` – Entry point and main loop controller

> ⚠️ You may have all functions in a single file (`shell.c`), or separate as modular components.

## 🛠️ Compilation

To compile the shell from multiple files:

```bash
gcc shell.c -o shell

