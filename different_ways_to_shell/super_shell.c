#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include<sysexits.h>

#define DEL "\n\t \v\f\r"

// old and current dir
static char old_dir[BUFSIZ] = "";
static char current_dir[BUFSIZ] = "";

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

// Get cwd function

void get_cwd(){
	char cwd[BUFSIZ];
	
	if(getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("get cwd failed");
	}

	strcpy(current_dir,cwd);

//	printf("[%s] $", cwd);
	fflush(stdout);
}

// 2. Shell Input Function
char *shell_get_input(void) {
    char *buf = NULL;
    size_t bufsize = 0;
    char cwd[BUFSIZ];

    //strcpy(old_dir,current_dir);
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
    }
    strcpy(current_dir,cwd);


    printf("[%s] $ ", cwd);
    fflush(stdout);

    //get_cwd();

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
	int background = 0 ;

	// detect & 
	int i = 0;
	while(args[i] != NULL){
		i++;
	}

	if(i > 0 && strcmp(args[i-1], "&") == 0){
		background = 1;
		args[i-1] = NULL; // remove & token
	}

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
		if(background)
		{
			printf("[backgroud pid %d]\n", pid);
			//dont wait, this will put the process in backgound until it is finished
		}
		else{
			// Parent process: wait for child to finish
			int status;
			waitpid(pid, &status, 0);
		}
	}
}

// Execute cd home
void cd_home()
{
	const char *home = getenv("HOME");
	if(home == NULL)
	{
		perror("cd $HOME error");
	}
	else{
		if(chdir(home) !=0)
		{
			perror("cd error");
		}
	}

}

// Execute cd command
void execute_cd(char **args)

{

	//strcpy(old_dir, current_dir);
	if(args[1] == NULL)
	{
		strcpy(old_dir, current_dir);
		cd_home();
		get_cwd();
		return;
	}
	else if(strcmp(args[1], "-") == 0)
	{
		//strcpy(old_dir, current_dir);
		get_cwd();
		if(chdir(old_dir) != 0)
		{
			perror("cd failed");
		//printf(" - ");
		}
		printf("%s\n",old_dir);
		//get_cwd();
		strcpy(old_dir, current_dir);
		return;
	}
	else if(strcmp(args[1], "/") == 0)
	{
		strcpy(old_dir, current_dir);
		if(chdir(args[1]) != 0){
			perror("cd failed");
		}
		get_cwd();
		return;
	}
	else{
		strcpy(old_dir, current_dir);
		if(chdir(args[1]) != 0)
		{
			perror("cd failed");
		}
		get_cwd();
		return;
	}


}

// Execute exit
//void execude_exit(void)
//{

//}

// 5. Main Shell Loop
int main() {
    printbanner();

    char *input;
    char **args;

    while ((input = shell_get_input())) {
        args = shell_split_line(input);
	
	if(args[0] == NULL)
	{
		free(args);
		free(input);
		continue;
	}

        // Execute command
	
	if((strcmp(args[0], "cd") == 0)){
		execute_cd(args);
		continue;
	}

	// exit command
	else if(strcmp(args[0], "exit") == 0){
		printf("exiting.....\n");
		exit(0);
	}
	else{
		int i = 0;
		while(args[i] != NULL)
		{
			

		}
		execute_shell(args);
		continue;
	}
	free(args);
	free(input);

    }
    return EXIT_SUCCESS;
}
