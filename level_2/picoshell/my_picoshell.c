#include <stdio.h>
#include <unistd.h>

int	picoshell(char **cmds[])
{
	int i;
	int fd[2];
	int pid;

	i = 0;
	pipe(fd);
	while(cmds[i])
	{
		pid = fork()
		if (pid == 0)
		{
			
		}
	}
}
