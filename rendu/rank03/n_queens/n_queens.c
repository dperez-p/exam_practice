#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int	ft_abs(int n)
{
	if(n < 0)
		return -n;
	return n;
}

int	is_safe(int *board, int colmn, int row)
{
	int	i = 0;
	while (i < colmn)
	{
		if (board[i] == row || ft_abs(board[i] - row) == ft_abs(i - colmn))
			return 0;
		i++;
	}
	return 1;
}


void	solve(int n, int *board, int column)
{
	int	i;
	int	row;
	if (column == n)
	{
		i = 0;
		while(i < n)
		{
			printf("%d", board[i]);
			if (i + 1 < n)
				printf (" ");
			i++;
		}
		printf("\n");
		return ;
	}
	row = 0;
	while (row < n)
	{
		if (is_safe(board, column, row))
		{
			board[column] = row;
			solve(n, board, column + 1);
		}
		row++;
	}
}

int	main(int ac, char **av)
{
	if(ac != 2)
		return 1;
	int nbr = atoi(av[1]);
	if (!nbr)
		return 0;
	int *board = (int *)calloc(nbr, sizeof(int));
	if(!board)
		return 1;
	solve(nbr, board, 0);
	free(board);
	return 0;
}
