#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Arena
{
	size_t capacity;
	size_t size;
	void *data;
} Arena;

typedef struct MyData
{
	char name[5];
	int nums[8];
} MyData;

Arena arena_new(size_t capacity);
void *arena_alloc(Arena *a, size_t size);
int arena_free(Arena *a);

Arena arena_new(size_t capacity)
{
	void *data = calloc(1, capacity);
	return (Arena) {
		.capacity = capacity,
		.size = 0,
		.data = data
	};
}

void *arena_alloc(Arena *a, size_t size)
{
	void *data;
	if (a->size + size > a->capacity)
	{
		return NULL;
	}
	if (size < 1)
	{
		return NULL;
	}

	data = (uint8_t*)a->data + (size - 1);
	a->size += size;
	return data;
}

int arena_free(Arena *a)
{
	if (a == NULL)
	{
		return 1;
	}

	free(a->data);
	a->data = NULL;
	a->size = 0;
	a->capacity = 0;
	return 0;
}

int main(void)
{
	Arena my_memory = arena_new(sizeof(MyData) * 2);
	MyData *my_data = arena_alloc(&my_memory, sizeof(MyData));
	MyData *my_data_2 = arena_alloc(&my_memory, sizeof(MyData));
	if (my_data_2 == NULL)
	{
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

	arena_free(&my_memory);
}
