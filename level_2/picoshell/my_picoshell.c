#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	int i;
	int fd[2] = {-1,-1};
	int prev_pid;
	int pid;

	prev_pid = 0;
	while(cmds[i])
	{
		if (cmds[i + 1])
			pipe(fd);
		pid = fork();
		if(pid == 0)
		{
			dup2(prev_pid, STDIN_FILENO);
			if (cmds[i + 1])
				dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{
			if (prev_pid != 0)
				close (prev_pid);
			prev_pid = fd[0];
			close(fd[1]);
		}
		i++;
	}
	wait(NULL);
}

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
static int	count_cmds(int argc, char **argv)
{
	int	count = 1;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "|") == 0)
			count++;
	}
	return (count);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (fprintf(stderr, "Usage: %s cmd1 [args] | cmd2 [args] ...\n", argv[0]), 1);

	int	cmd_count = count_cmds(argc, argv);
	char	***cmds = calloc(cmd_count + 1, sizeof(char **));
	if (!cmds)
		return (perror("calloc"), 1);

	int	i = 1, j = 0;
	while (i < argc)
	{
		int	len = 0;
		while (i + len < argc && strcmp(argv[i + len], "|") != 0)
			len++;
		cmds[j] = calloc(len + 1, sizeof(char *));
		if (!cmds[j])
			return (perror("calloc"), 1);
		for (int k = 0; k < len; k++)
			cmds[j][k] = argv[i + k];
		cmds[j][len] = NULL;
		i += len + 1;
		j++;
	}
	cmds[cmd_count] = NULL;

	int	ret = picoshell(cmds);
	perror("picoshell");

	// Clean up
	for (int i = 0; cmds[i]; i++)
		free(cmds[i]);
	free(cmds);

	return (ret);
}
