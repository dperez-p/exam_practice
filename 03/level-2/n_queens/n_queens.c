#include <stdlib.h>
#include <stdio.h>

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return n;
}


int	is_safe(int *board, int current_col, int row_to_check)
{
	int	i;
	i = 0;
	while (i < current_col)
	{
		if(board[i] == row_to_check || ft_abs(board[i] - row_to_check) == ft_abs(i - current_col))
					return 0;
	i++;
	}
	return 1;
}


void	solve(int n, int *board, int current_col)
{
	int	row;
	int	i;
	if (current_col == n)
	{
		i = 0;
		while (i < n)
		{
			printf("%d", board[i]);
			if (i + 1 < n)
				printf(" ");
			i++;
		}
		printf("\n");
		return ;
	}
	row = 0;
	while (row < n)
	{
		if (is_safe(board, current_col, row))
		{
			board[current_col] = row;
			solve(n, board, current_col + 1);
		}
		row++;
	}
}

int	main(int ac, char **av)
{
	int	n;
	int	*board;
	if (ac != 2)
		return 1;
	n = atoi(av[1]);
	board = (int *)calloc(n, sizeof(int));
	if (!board)
		return 1;

	solve(n, board, 0);
	free(board);
	return 0;
}
