#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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

char *ft_strdup(char *str)
{
	int length;
	int i;
	char *ptr;

	for (length = 0; str[length]; length++);
	ptr = malloc(length + 1);
	if (!ptr)
		return (NULL);
	for (i = 0; str[i]; i++)
		ptr[i] = str[i];
	ptr[i] = 0;
	return (ptr);
}

int parse_integer(json *file, FILE *stream)
{
	int integer;

	if (fscanf(stream, "%d", &integer) > 0)
	{
		file->type = INTEGER;
		file->integer = integer;
		printf("im here this is integer : %d\n", integer);
		return (SUCCESS);
	}
	else
		return (FAILURE);
}

char *parse_string(FILE *stream)
{
	char buffer[10000];
	int i = 0;
	char c;

	if (!expect(stream, '"')) return (NULL);
	while(peek(stream) != '"' && peek(stream) != EOF)
	{
		c = getc(stream);
		if (c == '\\')
			c = getc(stream);
		buffer[i] = c;
		i++;
	}
	if (!expect(stream, '"')) return (NULL);
	buffer[i] = 0;
	return (ft_strdup(buffer));
}

int fill_key(json *file, FILE *stream, int i)
{
	char *str;
	pair *pairs;

	str = parse_string(stream);
	if (str)
	{
		pairs = realloc(file->map.data, i + 1);
		if (!pairs)
			return (FAILURE);
		file->map.data = pairs;
		file->map.data[i].key = str;
		return (SUCCESS);
	}
	else
		return (FAILURE);
}

int parse_map(json *file, FILE *stream)
{
	int i = 0;
	char *str;

	if (!expect(stream, '{')) return (FAILURE);
	file->type = MAP;
	file->map.data = NULL;
	while(peek(stream) != '}')
	{
		if (!fill_key(file, stream, i))
			return (FAILURE);
		if (!expect(stream, ':')) return (FAILURE);
		if (peek(stream) == '{')
		{
			if (!parse_map(&file->map.data[i].value, stream))
				return (FAILURE);
		}
		else if (isdigit(peek(stream)))
		{
			printf("im here this is i : %d\n", i);
			if (!parse_integer(&file->map.data[i].value, stream))
				return (FAILURE);
			printf("im here this is the value after returning : %d\n", file->map.data[i].value.integer);
		}
		else if (peek(stream) == '"')
		{
			str = parse_string(stream);
			if (str)
			{
				file->map.data[i].value.type = STRING;
				file->map.data[i].value.string = str;
			}
			else
				return (FAILURE);
		}
		else
			return (FAILURE);
		if (peek(stream) != '}')
		{
			if (!expect(stream, ','))
				return (FAILURE);
		}
		i++;
	}
	file->map.size = i;
	return (SUCCESS);
}

int argo(json *file, FILE *stream)
{
	char *str;
	if (isdigit(peek(stream)))
		return (parse_integer(file, stream));
	if (peek(stream) == '"')
	{
		str = parse_string(stream);
		if (str)
		{
			file->type = STRING;
			file->string = str;
			return (SUCCESS);
		}
		else
			return (FAILURE);
	}
	if (peek(stream) == '{')
		return (parse_map(file, stream));
	return (FAILURE);
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
}
