#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> // change this to <stdlib.h>

#define SUCCESS 1
#define FAILURE -1

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

void	free_json(json j);
int	argo(json *dst, FILE *stream);

int	peek(FILE *stream)
{
	int	c = getc(stream);
	if (c != EOF)
		ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("Unexpected token '%c'\n", peek(stream));
	else
		printf("Unexpected end of input\n");
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

void	serialize(json j)
{
	switch (j.type)
	{
		case INTEGER:
			printf("%d", j.integer);
			break ;
		case STRING:
			putchar('"');
			for (int i = 0; j.string[i]; i++)
			{
				if (j.string[i] == '\\' || j.string[i] == '"')
					putchar('\\');
				putchar(j.string[i]);
			}
			putchar('"');
			break ;
		case MAP:
			putchar('{');
			for (size_t i = 0; i < j.map.size; i++)
			{
				if (i != 0)
					putchar(',');
				serialize((json){.type = STRING, .string = j.map.data[i].key});
				putchar(':');
				serialize(j.map.data[i].value);
			}
			putchar('}');
			break ;
	}
}

char *ft_strdup(char *s)
{
	int len;
	for (len=0; s[len]; len++);
	char *new = (char *)malloc(sizeof(char) * len + 1);
	if (!new) return (NULL);
	for (int i=0; s[i]; i++) new[i] = s[i];
	new[len] = 0;
	return (new);
}

int parse_integer(json *dst, FILE *stream)
{
	if (!fscanf(stream, "%d", &dst->integer))
		return (unexpected(stream), FAILURE);
	dst->type = INTEGER;
	return (SUCCESS);
}

int parse_string(json *dst, pair *pair, FILE *stream)
{
	int c;
	char buffer[7000];
	int i = 0;

	if (!expect(stream, '"')) return (FAILURE);
	while (peek(stream) != '"' && peek(stream) != EOF)
	{
		c = getc(stream);
		if (c == '\\') c = getc(stream);
		if (c == EOF) return (unexpected(stream), FAILURE);
		buffer[i++] = c;
	}
	buffer[i] = 0;
	if (!expect(stream, '"')) return (FAILURE);
	if (dst)
	{
		dst->type = STRING;
		dst->string = ft_strdup(buffer);
		if (!dst->string) return (FAILURE);
	}
	else
	{
		pair->key = ft_strdup(buffer);
		if (!pair->key) return (FAILURE);
	}
	return (SUCCESS);
}

int free_pairs(pair *pairs, int count)
{
	if (!pairs)
		return (FAILURE);
	for (int i = 0; i < count; i++)
	{
		if (pairs[i].key)
			free(pairs[i].key);
		free_json(pairs[i].value);
	}
	free(pairs);
	return (FAILURE);
}

void init_pair(pair *pair)
{
	pair->key = NULL;
	pair->value.string = NULL;
	pair->value.map.data = NULL;
}

int parse_map(json *dst, FILE *stream)
{
	int pair_num = 0;
	pair *pairs = NULL;

	if (!expect(stream, '{'))
		return (free_pairs(pairs, pair_num));
	while (peek(stream) != '}' && peek(stream) != EOF)
	{
		pair_num++;
		pairs = (pair *)realloc(pairs, sizeof(pair) * pair_num);
		if (!pairs)
			return (FAILURE);
		init_pair(&pairs[pair_num - 1]);
		if (parse_string(NULL, &pairs[pair_num - 1], stream) == FAILURE)
			return (free_pairs(pairs, pair_num));
		if (!expect(stream, ':'))
			return (free_pairs(pairs, pair_num));
		if (argo(&(pairs[pair_num - 1]).value, stream) == FAILURE)
			return (free_pairs(pairs, pair_num));
		if (peek(stream) != '}' && !expect(stream, ','))
			return (free_pairs(pairs, pair_num));
	}
	if (!expect(stream, '}'))
		return (free_pairs(pairs, pair_num));
	dst->type = MAP;
	dst->map.data = pairs;
	dst->map.size = pair_num;
	return (SUCCESS);
}

int	argo(json *dst, FILE *stream)
{
	int c = peek(stream);

	if (c == '"')
		return (parse_string(dst, NULL, stream));
	if (c == '{')
		return (parse_map(dst, stream));
	if (isdigit(c) || c == '+' || c == '-')
		return (parse_integer(dst, stream));
	return (unexpected(stream), FAILURE);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
	free_json(file); // in case of everything goes well, we need to free our json
}