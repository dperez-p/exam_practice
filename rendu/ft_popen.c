#include <unistd.h>
#include <stdlib.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int	pfd[2]; // array de tuberia, pfd[0] lectura, pfd[1] escritura
	pid_t	pid;	// guardado de ID

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(pfd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(pfd[0]);
			dup2(pfd[1], 1);
			close(pfd[1]);
		}
		else
		{
			close(pfd[1]);
			dup2(pfd[0], 0);
			close(pfd[0]);
		}
		execvp(file, argv);
		exit (1);
	}
	if (type == 'r')
	{
		close(pfd[1]);
		return (pfd[0]);
	}
	else
	{
		close(pfd[0]);
		return (pfd[1]);
	}
}
