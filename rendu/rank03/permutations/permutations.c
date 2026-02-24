#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_strchar(char *s, char c)
{
	while(*s)
	{
		if(*s == c)
			return 1;
		s++;
	}
	return 0;
}

int	ft_strlen(char *s)
{
	int	i = 0;
	while(s[i])
		i++;
	return i;
}

void	bubble_sort(char *src, int len)
{
	int i, j;
	char	tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - 1 - i)
		{
			if(src[j] > src[j + 1])
			{
				tmp = src[j];
				src[j] = src[j + 1];
				src[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}


void	solve(char *src, char *res, int len, int i)
{
	if (i == len)
	{
		puts(res);
		return ;
	}
	int	n = 0;
	while (n < len)
	{
		if(!ft_strchar(res, src[n]))
		{
			res[i] = src[n];
			res[i + 1] = '\0';
			solve(src, res, len, i + 1);
			res[i] = '\0';
		}
		n++;
	}
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	int	len = ft_strlen(av[1]);
	char	*res = calloc(len + 1, sizeof(char));
	if (!res)
		return 1;
	bubble_sort(av[1], len);
	solve(av[1], res, len, 0);
	free(res);
	return 0;
}
