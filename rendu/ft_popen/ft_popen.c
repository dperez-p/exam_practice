/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 18:03:17 by dperez-p          #+#    #+#             */
/*   Updated: 2026/02/15 18:03:17 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* pfd[0] = lectura
	pfd[1] = escritura */

#include <stdlib.h>
#include <unistd.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	pid_t	pid;
	int		pfd[2];

if (!file || !argv || (type != 'w' && type != 'r'))
{
	return -1;
}
if (pipe(pfd) == -1)
{
	return -1;
}
pid = fork();
if (pid == -1)
{
	close(pfd[0]);
	close(pfd[1]);
	return -1;
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
	return -1;
}
if (type == 'r')
{
	close(pfd[1]);
	return (pfd[0]);
}
else
{
	close(pfd[0]);
	return(pfd[1]);
}
}
