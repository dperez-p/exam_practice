
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


# define BUFFER_SIZE 42



void	ft_filter(char	*src, char *target)
{
	int	i,j,k;
	int	target_len = strlen(target);

	i = 0;
	while (src[i])
	{
		j = 0;
		while(src[i + j] == target[j])
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
			write (1, &src[i], 1);
			i++;
	
		}
	}
}



int	main(int ac, char **av)
{
	if (ac != 2 || !av[1][0])
		return 1;
	char	b[BUFFER_SIZE];
	char	*new_buffer = NULL;
	char	*res = NULL;
	ssize_t	bytes_read;
	ssize_t total_read;

	while ((bytes_read = read(0, b, BUFFER_SIZE)) > 0)
	{
		new_buffer = realloc(res, total_read + bytes_read);
		if (!new_buffer)
		{
			free (res);
			perror("realloc");
			return 1;
		}
		res = new_buffer;
		memcpy(res + total_read, b, bytes_read);
		total_read += bytes_read;
	}
	if (bytes_read < 0 || total_read <= 0)
	{
		free (res);
		perror("read");
		return 1;
	}
	ft_filter(res, av[1]);
	free(res);
	return 0;
}
