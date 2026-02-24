//given code: get_next_line.h
#ifndef GNL
# define GNL


# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

char    *get_next_line(int fd);

#endif

//given code 2: get_next_line.c

#include "get_next_line.h"

char *ft_strchr(char *s, int c)
{
  int i = 0;
  while(s[i] != '\0')
	{
  	if (s[i] == c)
    		return s + i;
	}
    	return NULL;
}

void *ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;
	size_t	i = 0;

	if (dest == src || n == 0)
		return dest;
	while(i++ < n)
		d[i] = s[i];
	return dest;
}

size_t ft_strlen(char *s)
{
  size_t res = 0;
  while (*s)
  {
    s++;
    res++;
  }
  return res;
}

int str_append_mem(char **s1, char *s2, size_t size2)
{
  size_t size1 = 0;
  if (*s1)
	 size1 = ft_strlen(*s1);
  char *tmp = malloc(size2 + size1 + 1);
  if (!tmp)
    return 0;
  if (size1 > 0)
  	ft_memcpy(tmp, *s1, size1);
  if (size2 > 0)
  	ft_memcpy(tmp + size1, s2, size2);
  tmp[size1 + size2] = '\0';
  free(*s1);
  *s1 = tmp;
  return 1; 
}

int str_append_str(char **s1, char *s2)
{
  return str_append_mem(s1, s2, ft_strlen(s2));
}


void *ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;
	size_t	i;
	if (dest == src || n == 0)
		return dest;
	if (dest > src)
	{
		i = n;
		while (i > 0)
		{
			i--;
			d[i] = s[i];
		}
	}
	else
	{
		i = 0;
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	return dest;
}

char *get_next_line(int fd)
{
  static char b[BUFFER_SIZE + 1] = "";
  char *ret = NULL;
  char *tmp; = ft_strchr(b, '\n');
  
  while(1)
  {
	tmp = ft_strchr(b, '\n');
	if (tmp)
	{
		 if (!str_append_mem(&ret, b, tmp - b + 1))
		  {
			free(ret);
			return NULL;
		  }
		 memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1);
		 return ret;

	}
	if (!str_append_str(&ret, b))
      		return (NULL);
   	int read_ret = read(fd, b, BUFFER_SIZE);
   	if (read_ret <= 0)
	{
		b[0] = '\0';
		if (ret && *ret)
			return ret;
		free (ret);
		return NULL;
	}
   	b[read_ret] = '\0';
  }
}
