/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo_reference.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:54:07 by dperez-p          #+#    #+#             */
/*   Updated: 2026/02/16 16:56:46 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int argo(json *dst, FILE *stream);

/* ** 1. PARSE_INTEGER: Extrae un número entero.
** Es la función más atómica (no llama a nadie más).
*/
static int  parse_integer(json *dst, FILE *stream)
{
    dst->type = INTEGER; // Marcamos el tipo en la estructura
    // fscanf salta espacios (si hubiera) y lee el número
    // devolviendo 1 si tuvo éxito.
    if (fscanf(stream, "%d", &dst->integer) != 1)
        return -1;
    return 1;
}

/* ** 2. PARSE_STRING: Lee caracteres entre comillas y maneja escapes.
** Aquí practicamos la gestión de memoria dinámica con realloc.
*/
static int  parse_string(char **dst, FILE *stream)
{
    // Consumimos la comilla inicial. Si no hay, error.
    if (!expect(stream, '"'))
        return (-1);

    size_t  len = 0;
    char    *s = malloc(1); // Reservamos espacio para el terminador nulo \0
    if (!s) return -1;
    s[0] = '\0';

    int c;
    // Leemos hasta la comilla de cierre o el fin del archivo
    while ((c = getc(stream)) != '"' && c != EOF)
    {
        // Si detectamos \, ignoramos la barra y leemos el siguiente carácter
        if (c == '\\')
        {
            c = getc(stream);
            if (c != '"' && c != '\\') // Solo permitimos escapar \" y \\
            {
                free(s);
                return (-1);
            }
        }
        // Redimensionamos el array: tamaño actual + nueva letra + \0
        char *tmp = realloc(s, len + 2);
        if (!tmp) { free(s); return -1; }
        s = tmp;

        s[len++] = (char)c; // Guardamos el carácter e incrementamos el contador
        s[len] = '\0';      // Mantenemos la cadena siempre terminada en nulo
    }

    if (c == EOF) // Si el bucle terminó por EOF, faltaba la comilla de cierre
    {
        free(s);
        printf("unexpected end of input\n");
        return (-1);
    }
    *dst = s; // Pasamos la dirección de la memoria reservada al puntero destino
    return 1;
}

/* ** 3. PARSE_MAP: La función recursiva que maneja el objeto { "k": v }.
*/
static int  parse_map(json *dst, FILE *stream)
{
    dst->type = MAP;        // Inicializamos el tipo para que free_json sepa qué hacer
    dst->map.data = NULL;
    dst->map.size = 0;

    if (!expect(stream, '{'))
        return (-1);
    
    // Caso especial: El mapa está vacío
    if (accept(stream, '}'))
        return (1);

    while (1)
    {
        // Agrandamos el array de parejas 'pair' (clave y valor)
        dst->map.data = realloc(dst->map.data, sizeof(pair) * (dst->map.size + 1));
        pair *actual = &dst->map.data[dst->map.size];

        // A. La clave siempre es un string
        if (parse_string(&actual->key, stream) != 1)
            return -1;
        // B. Debe haber dos puntos entre clave y valor
        if (!expect(stream, ':'))
            return -1;
        // C. RECURSIVIDAD: El valor puede ser cualquier tipo JSON, llamamos a argo()
        if (argo(&actual->value, stream) != 1)
            return -1;

        dst->map.size++;

        // Si hay una coma, el bucle continúa para leer el siguiente par
        if (accept(stream, ','))
            continue ;
        // Si no hay coma, debe cerrarse con llave
        if (expect(stream, '}'))
            return (1);
        
        return -1;
    }
}

/* ** 4. ARGO: El director de orquesta.
** Decide qué especialista llamar basándose en el primer carácter.
*/
int argo(json *dst, FILE *stream)
{
    int c = peek(stream); // Miramos qué viene sin "sacarlo" del archivo

    if (c == EOF)
    {
        unexpected(stream);
        return (-1);
    }
    // Lógica de selección según el estándar JSON
    if (c == '"')
    {
        dst->type = STRING;
        return (parse_string(&dst->string, stream));
    }
    if (c == '{')
    {
        // parse_map se encarga de poner el tipo MAP
        return (parse_map(dst, stream));
    }
    if (isdigit(c) || c == '-')
    {
        // parse_integer se encarga de poner el tipo INTEGER
        return (parse_integer(dst, stream));
    }
    // Si no es ninguna de las anteriores, el token es inválido
    unexpected(stream);
    return (-1);
}