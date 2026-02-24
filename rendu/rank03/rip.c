#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	min_to_rm(char *s)
{
	int i = 0, j = 0;
	while(*s)
	{
		if (*s == '(')
			i++;
		else if (*s == ')')
		{
			if (i)
				i--;
			else
				j++;
		}
		s++;
	}
	return i + j;
}

void	solve(char *src, int rm, int done, int pos)
{
	if (!src[pos])
	{	
		if (done == rm && !min_to_rm(src))
			puts(src);
		return ;
	}
	if (done > rm)
	{
		return ;
	}
	if (src[pos] == '(' || src[pos] == ')')
	{
		char	tmp = src[pos];
		src[pos] = ' ';
		solve(src, rm, done + 1, pos + 1);
		src[pos] = tmp;
	}
	solve(src, rm, done, pos + 1);
}

int	main(int ac, char **av)
{
	if(ac != 2)
		return 1;
	int rm = min_to_rm(av[1]);
	if (!rm)
	{
		puts(av[1]);
		return 0;
	}
	solve(av[1], rm, 0, 0);
	return 0;
}
