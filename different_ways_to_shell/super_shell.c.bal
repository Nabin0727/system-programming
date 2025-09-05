#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>

#define DEL "\n\t \v\f\r"

// printing banner
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

// shell input function
char *shell_get_input(void)
{
	char *buf;
	size_t bufsize;
	char cwd[BUFSIZ];
	
	// Getting current directory through getcwd
	if (NULL == getcwd(cwd,sizeof(cwd)))
		perror("\ngetcwd failed\n");
	
	buf = NULL;
	printf("[%s] $ ", cwd);
	fflush(stdout); // this clears buffer by pushing its content, hepls in smooth '$' print to stdout
	
	if(getline(&buf, &bufsize, stdin) == -1)
	{
		buf = NULL;
		if(feof(stdin))
			printf("[EOF], recevied! Exiting shell!");
		else
			printf("Getline failed");

		free(buf);
		return NULL;

	}
	return buf;
}
// argument vector
//
char **shell_split_line(char *line)
{
	char **tokens;
	unsigned int position;
	size_t bufsize;

	bufsize = BUFSIZ;

	tokens = malloc(tokens, sizeof(tokens);
	
	for( char *token = strtok(line, DEL); token; token=strtok(NULL, DELL))
		{
			tokens[position++] = token;
			if (position >=bufsize)
			{
				bufsize *= 2;
				tokens = realloc(tokens, sizeof());
			}	
		}
		tokens[position] = NULL;
		return tokens;
}

int main()
{	
	// calling banner
	printbanner();
	//REPL
	// Read -> Evaluate -> Print/Execute -> Loop
	
	char *input, **args;

	while((input=shell_get_input())) 
	{	
		// get line
		printf("%s", input);

		
		// get tokens --> parsing --> evluating
		// argument vector
		// ls -al file
		// tokens --> ls
		// 	---> -la
		// 	---> file
		args = shell_split_line(input);
		// exectute
	}

	free(input);
	
	//retun 0;
	return EXIT_SUCCESS;
}

