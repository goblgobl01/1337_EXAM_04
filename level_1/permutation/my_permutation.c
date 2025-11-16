#include <stdio.h>
#include <string.h>

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

void swap_characters(char *a, char *b)
{
	char temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void sort_string(char *str, int length)
{
	int		i = 0;
	int		j;

	while(str[i])
	{
		j = i + 1;
		while(j < length)
		{
			if (str[i] > str[j])
				swap_characters(&str[i], &str[j]);
			j++;
		}
		i++;
	}
}

int permutations(char *str, int length)
{
	int i = length - 2;
	int j = length - 1;

	while(str[i] > str[i + 1])
		i--;
	if (i == -1)
		return (0);
	while(str[j] < str[i])
		j--;
	swap_characters(&str[i], &str[j]);
	i++;
	j = length - 1;
	while(i < j)
	{
		swap_characters(&str[i], &str[j]);
		i++;
		j--;
	}
	return (1);
}

int	main(int ac , char **av)
{
	if(ac == 2)
	{
		int length = ft_strlen(av[1]);
		sort_string(av[1], length);
		puts(av[1]);
		while(permutations(av[1], length))
			puts(av[1]);
	}
}