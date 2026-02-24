/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 10:04:29 by dperez-p          #+#    #+#             */
/*   Updated: 2026/02/23 13:10:34 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *parse_expr(char **s);
node	*parse_term(char **s);
node	*parse_factor(char **s);

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node	*parse_factor(char **s)
{
	if (accept(s, '('))
	{
		node	*res = parse_expr(s);
		if (!res)
		{
			return NULL;
		}
		if (!expect(s, ')'))
		{
			destroy_tree(res);
			return NULL;
		}
		return	res;
	}
	if (isdigit(**s))
	{
		node	*num = new_node((node){VAL, **s - '0', NULL, NULL});
		(*s)++;
		return num;
	}
	unexpected(**s);
	return NULL;
}

node	*parse_term(char **s)
{
	node	*l = parse_factor(s);
	if (!l)
	{
		return NULL;  
	}
	while (accept(s, '*'))
	{
		node	*r = parse_factor(s);
		if (!r)
		{
			destroy_tree(l);
			return NULL;
		}
		l = new_node((node){MULTI, 0, l, r});
	}
	return l;
}

node	*parse_expr(char **s)
{
	node	*l = parse_term(s);
	if (!l)
	{
		return NULL;
	}
	while (accept(s, '+'))
	{
		node	*r = parse_term(s);
		if (!r) 
		{
			destroy_tree(l);
			return (NULL);
		}
		l = new_node((node){ADD, 0, l, r});
	}
	return (l);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	char	*str = argv[1];
	node *tree = parse_expr(&str);
	if (!tree)
		return (1);
	if (*str != '\0')
	{
		unexpected(*str);
		destroy_tree(tree);
		return 1;
	}
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
}
