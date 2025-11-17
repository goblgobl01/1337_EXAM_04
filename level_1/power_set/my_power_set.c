#include <stdio.h>
#include <stdlib.h>

int target;
int *set;
int set_size;
int arg_c;
char **arg_v;

void print_set()
{
	int i;
	
	i = 0;
	while(i < set_size)
	{
		printf("%d", set[i]);
		if(i < set_size - 1)
			printf(" ");
		i++;
	}
	printf("\n");
}

int check_set()
{
	int i;
	int sum;

	i = 0;
	sum = 0;
	while(i < set_size)
	{
		sum += set[i];
		i++;
	}
	return (sum);
}

void power_set(int pos)
{
	if (check_set() == target)
		print_set();

	for (int i = pos; arg_v[i]; i++)
	{
		set[set_size] = atoi(arg_v[i]);
		set_size++;
		power_set(i + 1);
		set_size--;
	}
}

int main(int ac, char **av)
{
	target = atoi(av[1]);
	int temp[ac - 2];
	set = temp;
	arg_v = av;
	arg_c = ac;
	power_set(2);
}
