
#include <stdlib.h>
#include <stdio.h>

void	print_subset(int *subset, int len)
{
	int	i;

	i = 0;
	while(i < len)
	{
		printf("%d", subset[i]);
		if(i < len - 1)
			printf(" ");
		i++;
	}
	printf("\n");
}

void	search(int target, int size, int *set, int *subset, int i, int len)
{
	if(i == size)
	{
		if (target == 0)
			print_subset(subset, len);
		return ;
	}
	subset[len] = set[i];
	search(target - set[i], size, set, subset, i + 1, len + 1);
	search(target, size, set, subset, i + 1, len);
}

int	main(int ac, char **av)
{
	int	target;
	int	size;
	int	*set;
	int	*subset;
	int	i;

	if (ac < 3)
		return 1;
	target = atoi(av[1]);
	size = ac - 2;
	set = calloc(size, sizeof(int));
	if(!set)
		return (free(set), 1);
	subset = calloc(size, sizeof(int));
	if(!subset)
	{
		free(set);
		return 1;
	}
	i = 0;
	while (i < size)
	{
		set[i] = atoi(av[i + 2]);
		i++;
	}
	search(target, size, set, subset, 0, 0);
}
