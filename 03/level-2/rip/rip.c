#include <unistd.h>
#include <stdio.h>

int	min_to_rm(char *s)
{
	int	o = 0, c = 0;
	while(*s)
	{
		if (*s == '(')
			o++;
		else if (*s == ')')
		{
			if (o)
				o--;
			else
				c++;
		}
		s++;
	}
	return o + c;
}

void	solve(char *s, int i, int rm, int done)
{
	if (!s[i])
	{
		if (done == rm && !min_to_rm(s))
			puts(s);
		return ;
	}
	if (done > rm)
		return ;
	if (s[i] == '(' || s[i] == ')')
	{
		char tmp = s[i];
		s[i] = ' ';
		solve(s, i + 1, rm, done + 1);
		s[i] = tmp;
	}
	solve(s, i + 1, rm, done);
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (write(1, "\n", 1), 0);
	int	rm = min_to_rm(av[1]);

	if (!rm) return (puts(av[1]), 0);
	solve(av[1], 0, rm, 0);
}
