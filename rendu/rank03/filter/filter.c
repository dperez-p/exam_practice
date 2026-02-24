
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

# define BUFFER_SIZE 42


void	ft_filter(char *src, char *target)
{
	int target_len = strlen(target);
	int	i,j, k;

	i = 0;
	while(src[i])
	{
		j = 0;
		while (src[i + j] == target[j])
			j++;
		if (j == target_len)
		{
			k = 0;
			while (k < j)
			{
				write(1, "*", 1);
				k++;
			}
			i += target_len;	
		}
		else
		{
			write (1 , &src[i], 1);
			i++;
		}
	}
}

int	main(int ac, char **av)
{
	char	b[BUFFER_SIZE];
	char	*new_buffer = NULL;
	char	*ret = NULL;
	ssize_t	bytes_read = 0;
	size_t	total_read;

	if (ac != 2 || av[1][0] == '\0')
		return 1;
	while((bytes_read = read(0, b, BUFFER_SIZE)) > 0)
	{
		new_buffer = realloc(ret, bytes_read + total_read + 1);
		if (!new_buffer)
		{
			free(ret);
			return 1;
		}
		ret = new_buffer;
		memmove (ret + total_read, b, bytes_read);
		total_read += bytes_read;
	}
	if (bytes_read < 0)
	{
		free (ret);
		perror ("realloc");
		return 1;
	}
	if (total_read <= 0)
	{
		free(ret);
		perror ("nothing read");
		return (0);
	}
	ft_filter(ret, av[1]);
	free(ret);
	return 0;
}
