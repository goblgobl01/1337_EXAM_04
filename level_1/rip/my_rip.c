#include <stdio.h>
#include <string.h>

int is_valid(char *str)
{
	int left,right,i;

	left = 0;
	right = 0;
	i = 0;
	while(str[i])
	{
		if(str[i] == '(')
			left++;
		if(str[i] == ')')
		{
			if(left > 0)
				left--;
			else
				return (0);
		}
		i++;
	}
	return (left == 0);
}

void rip(int left, int right, int index, char *str)
{
	if (left == 0 && right == 0)
	{
		if (is_valid(str))
			puts(str);
		return ;
	}
	int i = index;
	while(str[i])
	{
		if(str[i] == '(')
		{
			str[i] = ' ';
			rip(left - 1, right, i + 1, str);
			str[i] = '(';
		}
		if(str[i] == ')')
		{
			str[i] = ' ';
			rip(left, right - 1, i + 1, str);
			str[i] = ')';
		}
		i++;
	}
}

int main(int ac, char **av)
{
	if(ac != 2)
		return (0);
	int left,right,i;
	char *str;

	str = av[1];
	left = 0;
	right = 0;
	i = 0;
	while(str[i])
	{
		if(str[i] == '(')
			left++;
		if(str[i] == ')')
		{
			if(left > 0)
				left--;
			else
				right++;
		}
		i++;
	}
	rip(left, right, 0, str);
}