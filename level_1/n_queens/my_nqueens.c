#include <stdio.h>
#include <stdlib.h>

void print_solution(int *board, int length)
{
	int i = 0;
	while(i < length)
	{
		fprintf(stdout, "%d", board[i]);
		if (i != (length - 1))
			fprintf(stdout, " ");
		i++;
	}
	fprintf(stdout, "\n");
}

int is_valid(int *board, int row, int col, int length)
{
	int i = 0;

	while(i < row)
	{
		
	}
}


void nqueens(int *board, int row, int length)
{
	int col = 0;
	if (row == length)
		print_solution(board, length);
	while(col < length)
	{
		if (is_valid(board, row, col, length))
		{
			board[row] = col;
			nqueens(board, row + 1, length);
		}
		col++;
	}
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		int length = atoi(av[1]);
		int board[length];
		int row = 0;
		// nqueens(board, row, length);
		board[0] = 1;
		board[1] = 2;
		board[2] = 3;
		board[3] = 4;
		print_solution(board, length);
	}
}