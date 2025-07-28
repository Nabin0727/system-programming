#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

// shell input function
char *shell_get_input(void)
{
	char *buf;
	size_t bufsize;

	buf = NULL;
	printf("$ ");
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
int main()
{	
	//REPL
	// Read -> Evaluate -> Print/Execute -> Loop
	
	char *input;

	while(1) 
	{	
		input = shell_get_input();
		// get line
		printf("%s", input);

		
		// get tokens --> parsing --> evluating
		
		// exectute
	}

	free(input);
	
	//retun 0;
	return EXIT_SUCCESS;
}

