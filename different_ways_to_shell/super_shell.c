#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include<sysexits.h>

#define DEL "\n\t \v\f\r"

// 1. Banner Function
void printbanner() {
    printf("  ,----..              ,--,    ,--,                          ,---, \n");
    printf(" /   /   \\           ,--.'|  ,--.'|                       ,`--.' | \n");
    printf("|   :     :          |  | :  |  | :                      /    /  : \n");
    printf(".   |  ;. /          :  : '  :  : '                .---.:    |.' ' \n");
    printf(".   ; /--`    ,---.  |  ' |  |  ' |              /.  ./|`----':  | \n");
    printf(";   | ;      /     \\ '  | |  '  | |            .-' . ' |   '   ' ; \n");
    printf("|   : |     /    /  ||  | :  |  | :           /___/ \\: |   |   | | \n");
    printf(".   | '___ .    ' / |'  : |__'  : |__         .   \\  ' .   '   : ; \n");
    printf("'   ; : .'|'   ;   /||  | '.'|  | '.'|         \\   \\   '   |   | ' \n");
    printf("'   | '/  :'   |  / |;  :    ;  :    ;          \\   \\      '   : | \n");
    printf("|   :    / |   :    ||  ,   /|  ,   /            \\   \\ |   ;   |.' \n");
    printf(" \\   \\ .'   \\   \\  /  ---`-'  ---`-'              '---\"    '---'   \n");
    printf("  `---`      `----'                                                \n");
}

// 2. Shell Input Function
char *shell_get_input(void) {
    char *buf = NULL;
    size_t bufsize = 0;
    char cwd[BUFSIZ];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
    }

    printf("[%s] $ ", cwd);
    fflush(stdout);

    if (getline(&buf, &bufsize, stdin) == -1) {
        if (feof(stdin))
            printf("[EOF] received! Exiting shell!\n");
        else
            perror("getline failed");

        free(buf);
        return NULL;
    }

    return buf;
}

// 3. Split Input Line into Tokens (argv)
char **shell_split_line(char *line) {
    size_t bufsize = BUFSIZ;
    size_t position = 0;

    char **tokens = malloc(bufsize * sizeof(char *));
    if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, DEL);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize *= 2;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DEL);
    }

    tokens[position] = NULL; // Null-terminate the array
    return tokens;
}

// 4. Execute Shell
void execute_shell(char **args)
{	
	pid_t pid;
	pid = fork();

	if(pid < 0)
		{
			perror("fork failed");
			exit(EX_OSERR);
		}
	else if (pid == 0){
		if(execvp(args[0],args) == -1){
			perror("execvp failed");
			exit(EX_UNAVAILABLE);
		}
	}

	else{
		// Parent process: wait for child to finish
		int status;
		waitpid(pid, &status, 0);
	}
}

// Execute cd command
void execute_cd(char **args)

{
	if(args[1] == NULL)
	{
		printf("null");
	}
	else if(strcmp(args[1], "-") == 0)
	{
		printf(" - ");
	}
	else if(strcmp(args[1], "/") == 0)
	{
		printf(" root");
	}
	else{
		printf("custom");
	}


}

// 5. Main Shell Loop
int main() {
    printbanner();

    char *input;
    char **args;

    while ((input = shell_get_input())) {
        args = shell_split_line(input);


        // Execute command
	
	if((strcmp(args[0], "cd") == 0)){
		execute_cd(args);
	}
	else{
		execute_shell(args);
	}

        free(input);
        free(args);
    }

    return EXIT_SUCCESS;
}
