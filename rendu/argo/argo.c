/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 12:35:30 by dperez-p          #+#    #+#             */
/*   Updated: 2026/02/17 18:28:47 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
	}
}

int	argo(json *dst, FILE *stream);

static int	parse_integer(json *dst, FILE *stream)
{
	dst->type = INTEGER;
	if (fscanf(stream, "%d", &dst->integer) != 1)
	{
		return -1;
	}
	return 1;
}

static int	parse_string(char **dst, FILE *stream)
{
	if (!expect(stream, '"'))
	{
		return -1;
	}
	size_t	len = 0;
	char	*s = malloc(1);
	if (!s) return -1;
	s[0] = '\0';
	int	c;
	while ((c = getc(stream)) != '"' && c != EOF)
	{
		if (c == '\\')
		{
			c = getc(stream);
			if (c != '"' && c != '\\')
			{
				free(s);
				return -1;
			}
		}
		char	*tmp = realloc(s, len + 2);
		if (!tmp)
		{
			free(s);
			return -1;
		}
		s = tmp;
		s[len++] = (char)c;
		s[len] = '\0';
	}
	if (c == EOF)
	{
		free(s);
		printf("unexpected end of input\n");
		return -1;
	}
	*dst = s;
	return 1;
}

static int	parse_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;

	if (!expect(stream, '{'))
	{
		return -1;
	}
	if (accept(stream, '}'))
	{
		return 1;
	}
	while (1)
	{
		dst->map.data = realloc(dst->map.data, sizeof(pair) * (dst->map.size + 1));
		pair *actual = &dst->map.data[dst->map.size];
		if (parse_string(&actual->key, stream) != 1)
		{
			return -1;
		}
		if (!expect(stream, ':'))
		{
			return -1;
		}
		if (argo(&actual->value, stream) != 1)
		{
			return -1;
		}
		dst->map.size++;
		
		if (accept(stream, ','))
		{
			continue ;
		}
		if (expect(stream, '}'))
		{
			return 1;
		}
	}
}

int	argo(json *dst, FILE *stream)
{
	int	c = peek(stream);

	if (c == EOF)
	{
		unexpected(stream);
		return -1;
	}
	if (c == '"')
	{
		dst->type = STRING;
		return (parse_string(&dst->string, stream));
	}
	if (c == '{')
	{
		return (parse_map(dst, stream));
	}
	if (isdigit(c) || c == '-')
	{
		return (parse_integer(dst, stream));
	}
	unexpected(stream);
	return -1;
}