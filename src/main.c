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
	Arena my_memory = arena_new(sizeof(MyData) * 2);


	MyData *my_data = arena_alloc(&my_memory, sizeof(MyData), _Alignof(MyData));
	MyData *my_data_2 = arena_alloc(&my_memory, sizeof(MyData), _Alignof(MyData));
	if (my_data_2 == NULL)
	{
		printf("Could not allocate from arena: Current Size: %li, Capacity: %li\n", my_memory.size, my_memory.capacity);
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

	arena_reset(&my_memory);

	char *my_new_data = arena_alloc(&my_memory, sizeof(char) * 8, _Alignof(char));
	for (int ii = 0; ii < 8; ii++)
	{
		my_new_data[ii] = 'A' + ii;
	}
	my_new_data[8] = '\0';
	printf("%s\n", my_new_data);

	arena_free(&my_memory);
}
