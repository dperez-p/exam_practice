/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 18:03:57 by dperez-p          #+#    #+#             */
/*   Updated: 2026/02/20 16:40:09 by dperez-p         ###   ########.fr       */
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

// Factor: Maneja ( expresión ) O un número '0'-'9'
node	*parse_factor(char **s)
{
	if (accept(s, '('))
	{
		node *res = parse_expr(s);
		if (!res) return NULL;
		if (!expect(s, ')')) 
		{
			destroy_tree(res); // Liberamos lo que parseamos dentro antes de salir
			return NULL;
		}
		return res;
	}
		// ¡Faltaba esto! Si es un número:
		if (isdigit(**s))
		{
			node *num = new_node((node){VAL, **s - '0', NULL, NULL});
			(*s)++; // Muy importante: avanzar el puntero al siguiente carácter
			return num;
		}
		unexpected(**s);
		return NULL;
}

node	*parse_term(char **s)
{
		node *l = parse_factor(s);
		if (!l) return NULL;

		while (accept(s, '*'))
		{
			node *r = parse_factor(s);
			if (!r) 
			{
				destroy_tree(l); // <--- ¡Añadido! Si el segundo número falla, liberamos el primero
				return NULL;
			}
			l = new_node((node){MULTI, 0, l, r});
		}
		return l;
}

// Ahora recibe char **s para ser consistente
node	*parse_expr(char **s)
{
		node *l = parse_term(s);
		if (!l) return NULL;

		while (accept(s, '+'))
		{
			node *r = parse_term(s);
			if (!r) 
			{
				destroy_tree(l); // <--- ¡Añadido!
				return NULL;
			}
			l = new_node((node){ADD, 0, l, r});
		}
		return l;
}

int	main(int argc, char **argv)
{
		if (argc != 2)
			return (1);

		char *str = argv[1]; // Variable intermedia para poder pasar su dirección
		node *tree = parse_expr(&str); 

		if (!tree)
			return (1);
		
		// Verificación extra: si después de parsear sobra texto (ej: "1+2)"), es error
		if (*str != '\0') {
			unexpected(*str);
			destroy_tree(tree);
			return (1);
		}

		printf("%d\n", eval_tree(tree));
		destroy_tree(tree);
		return (0);
}
