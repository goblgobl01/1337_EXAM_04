#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h> // change this to <stdlib.h>


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

int parse_integer(json *file, FILE *stream)
{
	int integer;
	if (fscanf(stream, "%d", &integer))
	{
		file->type = INTEGER;
		file->integer = integer;
		return (1);
	}
	else
		return (-1);
}

char *ft_strdup(char *str)
{
	int length;
	char *ptr;
	int i;

	for(length = 0; str[length]; length++);
	ptr = malloc(length + 1);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while(i < length)
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}

int parse_string(json *file, FILE *stream)
{
	char buffer[10000];
	char c;
	char another_char;
	char next_char;
	int i;

	i = 0;
	getc(stream);
	c = getc(stream);
	while(c != '"')
	{
		if (c == '\\')
			c = getc(stream);
		if (c == '"')
		{
			buffer[i] = c;
			i++;
			break ;
		}
		if (c == EOF)
			return (-1);
		buffer[i] = c;
		c = getc(stream);
		i++;
	}
	buffer[i] = 0;

	file->type = STRING;
	file->string = ft_strdup(buffer);
	if (file->string == NULL)
		return (-1);
	return (1);
}

int argo(json *file, FILE *stream)
{
	if(isdigit(peek(stream)))
		return (parse_integer(file, stream));
	if (peek(stream) == '"')
		return (parse_string(file, stream));
}

int	main(int argc, char **argv)
{
	// if (argc != 2)
	// 	return 1;
	// char *filename = argv[1];
	FILE *stream = fopen("file.txt", "r");
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
}
