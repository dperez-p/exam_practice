#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



int	ft_strlen(char *src)
{
	int i = 0;
	while(src[i])
		i++;
	return i;
}

int	ft_strchr(char	*src, char c)
{
	int i = 0;
	while(src[i])
	{
		if (src[i] == c)
			return 1;
		i++;
	}
	return 0;
}

void bubble_sort(char	*src, int len)
{
	int	i,j;
	char	tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while(j < len - 1 - i)
		{
			if (src[j] > src[j + 1])
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


void	permutations(char *src, char *res, int pos, int len)
{
	int	i;
	if (pos == len)
	{
		puts(res);
		//write(1, "\n", 1);
		return ;
	}
	i = 0;
	while(i < len)
	{
		if (!ft_strchr(res, src[i]))
		{
			res[pos] = src[i];
			res[pos + 1] = '\0';
			permutations(src, res, pos + 1, len);
			res[pos] = '\0';
		}
		i++;
	}
}





int	main(int ac, char **av)
{
	char	*res = NULL;
	int	len = ft_strlen(av[1]);
	if (ac != 2)
		return 1;
	bubble_sort(av[1], len);
	res = calloc(len + 1, sizeof(char));
	if (!res)
		return 1;
	permutations(av[1], res, 0, len);
	free (res);
	return 0;
}
