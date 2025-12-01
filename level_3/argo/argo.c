#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


//added
#include <stdlib.h>
#include <ctype.h>

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

int parse_integer(json *file, FILE *stream)
{
	int integer;
	if (fscanf(stream, "%d", &integer) > 0)
	{
		file->type = INTEGER;
		file->integer = integer;
		return (1);
	}
	else
		return (unexpected(stream), -1);
}

char *parse_string(FILE *stream)
{
	int i;
	char buffer[10000];
	char c;
	char another_char;

	i = 0;
	if (!expect(stream, '"'))
		return (NULL);
	while(peek(stream) != '"' && peek(stream) != EOF)
	{
		c = getc(stream);
		if (c == '\\')
			c = getc(stream);
		if (c == EOF)
			return (unexpected(stream), NULL);
		buffer[i] = c;
		i++;
	}
	if (!expect(stream, '"'))
		return (NULL);
	buffer[i] = 0;
	return (ft_strdup(buffer));
}

int parse_map(json *file, FILE *stream)
{
	char *str;
	int i;
	pair *pairs;
	char c;
	int value;

	if (!expect(stream, '{'))
			return (-1);
	file->map.data = NULL;
	file->map.size = 0;
	file->type = MAP;
	i = 0;
	while(peek(stream) != '}')
	{
		str = parse_string(stream);
		if (str)
		{
			pairs = realloc(file->map.data, (i + 1) * sizeof(struct pair));
			file->map.data = pairs;
			file->map.data[i].key = str;
		}
		else
			return (-1);
		if (!expect(stream, ':'))
			return (-1);
		if (peek(stream) == '{')
		{
			if (parse_map(&file->map.data[i].value, stream) == -1)
				return (-1);
		}
		else if(peek(stream) == '"')
		{
			str = parse_string(stream);
			if (str)
			{
				file->map.data[i].value.type = STRING;
				file->map.data[i].value.string = str;
			}
			else
				return (-1);
		}
		else if(isdigit(peek(stream)))
		{
			value = parse_integer(&file->map.data[i].value, stream);
			c = peek(stream);
		}
		if (peek(stream) != '}')
		{
			if (!expect(stream, ','))
				return (-1);
		}
		i++;
	}
	file->map.size = i;
	return (1);
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
			file->string  = str;
			return (1);
		}
		else
			return (-1);
	}
	if (peek(stream) == '{')
		return (parse_map(file, stream));
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
	free_json(file); // in case of everything goes well, we need to free our json
}