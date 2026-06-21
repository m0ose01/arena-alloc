#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <arena.h>

typedef struct MyData
{
	char name[5];
	int nums[8];
} MyData;

int main(void)
{
	Arena *my_memory = arena_new(sizeof(MyData) * 2);

	MyData *my_data = arena_alloc(my_memory, sizeof(MyData), sizeof(long long));
	if (my_data == NULL)
	{
		printf("Could not allocate from arena\n");
		return 1;
	}
	MyData *my_data_2 = arena_alloc(my_memory, sizeof(MyData), sizeof(long long));
	if (my_data_2 == NULL)
	{
		printf("Could not allocate from arena\n");
		return 1;
	}
	char *hi = "hiii";
	for (int ii = 0; ii < 5; ii++)
	{
		my_data->name[ii] = hi[ii];
		my_data_2->name[ii] = hi[ii];
	}
	for (int ii = 0; ii < 8; ii++)
	{
		my_data->nums[ii] = (8 - 1) - ii;
		my_data_2->nums[ii] = ii;
	}

	printf("%i\n", my_data->nums[7]);
	printf("%s\n", my_data_2->name);

	arena_reset(my_memory);

	char *my_new_data = arena_alloc(my_memory, sizeof(char) * 8, sizeof(long long));
	for (int ii = 0; ii < 8; ii++)
	{
		my_new_data[ii] = 'A' + ii;
	}
	my_new_data[8] = '\0';
	printf("%s\n", my_new_data);

	Arena *large_arena = arena_new(sizeof(char) * (26 + 1) * 15);

	char *str;
	for (int ii = 0; ii < 100; ii++)
	{
		arena_reset(large_arena);
		const int count = sizeof(char) * (26);
		str = arena_alloc(large_arena, count, sizeof(char));
		if (str == NULL)
		{
			fprintf(stderr, "Error: could not allocate from arena.\n");
			break;
		}
		for (int jj = 0; jj < count; jj++)
		{
			str[jj] = 'A' + (jj % 26);
		}
		str[count - 1] = '\0';
	}

	arena_free(my_memory);
	arena_free(large_arena);
}
