#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// fork a child

// run an executable (using execvp)

// create a pipe between your program and the executed command

// return one file descriptor to the parent:

// 'r' → parent reads output of the command

// 'w' → parent writes input to the command

// If anything goes wrong → return -1.

int ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	// fd[0] read();
	// fd[1] write();
	int id;

	if (type != 'r' && type != 'w')
		return (-1);
	pipe(fd);
	id = fork();
	if (id == 0)
	{
		if(type == 'r')
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			execvp(file, argv);
			exit(1);
		}
		if(type == 'w')
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			execvp(file, argv);
			exit(1);
		}
	}
	else
	{
		if(type == 'r')
		{
			close(fd[1]);
			return (fd[0]);
		}
		else
		{
			close(fd[0]);
			return (fd[1]);
		}
	}
}

int main(void)
{
	int fd;
	char buffer[1024];
	ssize_t n;
	
	printf("=== Test 1: Read from 'ls' ===\n");
	char *argv_ls[] = {"ls", "-l", NULL};
	fd = ft_popen("ls", argv_ls, 'r');
	if (fd == -1) {
		perror("ft_popen failed");
		return 1;
	}
	while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[n] = '\0';
		printf("%s", buffer);
	}
	close(fd);
	
	printf("\n=== Test 2: Write to 'wc -l' ===\n");
	char *argv_wc[] = {"wc", "-l", NULL};
	fd = ft_popen("wc", argv_wc, 'w');
	if (fd == -1) {
		perror("ft_popen failed");
		return 1;
	}
	write(fd, "line 1\n", 7);
	write(fd, "line 2\n", 7);
	write(fd, "line 3\n", 7);
	close(fd);
	sleep(1); // wait for wc to finish
	
	printf("\n=== Test 3: Read from 'echo' ===\n");
	char *argv_echo[] = {"echo", "Hello from ft_popen!", NULL};
	fd = ft_popen("echo", argv_echo, 'r');
	if (fd == -1) {
		perror("ft_popen failed");
		return 1;
	}
	n = read(fd, buffer, sizeof(buffer) - 1);
	buffer[n] = '\0';
	printf("Output: %s", buffer);
	close(fd);
	
	printf("\n=== Test 4: Write to 'grep hello' ===\n");
	char *argv_grep[] = {"grep", "hello", NULL};
	fd = ft_popen("grep", argv_grep, 'w');
	if (fd == -1) {
		perror("ft_popen failed");
		return 1;
	}
	write(fd, "hello world\n", 12);
	write(fd, "goodbye world\n", 14);
	write(fd, "hello again\n", 12);
	close(fd);
	sleep(1);
	
	printf("\n=== Test 5: Invalid type ===\n");
	fd = ft_popen("ls", argv_ls, 'x');
	if (fd == -1)
		printf("Correctly returned -1 for invalid type\n");
	
	return 0;
}




/*
=== Test 1: Read from 'ls' ===
(shows directory listing)

=== Test 2: Write to 'wc -l' ===
3

=== Test 3: Read from 'echo' ===
Output: Hello from ft_popen!

=== Test 4: Write to 'grep hello' ===
hello world
hello again

=== Test 5: Invalid type ===
Correctly returned -1 for invalid type
*/