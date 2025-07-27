#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>


int main(int ac, char **av)
{	
	(void)ac;
	int status;

	// child process
	if (fork() ==0)
	{
		execvp(av[1], av + 1);
	}

	wait(&status);


	//retun 0;
	return EXIT_SUCCESS;
}

