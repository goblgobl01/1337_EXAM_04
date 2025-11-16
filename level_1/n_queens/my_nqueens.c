#include <stdio.h>
#include <stdlib.h>

int *board;

void print_solution(int length)
{
	int i;

	i = 0;
	while(i < n)
	{
		printf("%d", board[i]);
		if (i != (length - 1))
			printf(" ");
		i++;
	}
	printf("\n");
}

int absolute(int value)
{
	if (value < 0)
		return (-value);
	else
		return (value);
}

int is_safe(int col, int row, int length)
{
	int i = 0;

	while(i < col)
	{
		if(board[i] = row)
			return (0)
		if (absolute(i - row) == absolute(board[i] - col))
			return (0);
		i++;
	}
	return (1);
}

void n_queen(int col, int length)
{
	int i;

	if (col == length)
		print_solution(length);
	i = 0;
	while(i < length)
	{
		if(is_safe(i, length))
	}
}

int	main(int ac, char **av)
{
	int i;
	int length;

	if (ac == 2)
	{
		length = atoi(av[1]);
		int temp[length];
		board = temp;
		n_queen(0, length)
	}
}