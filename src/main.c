#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Arena
{
	size_t capacity;
	size_t size;
	void *data;
	void *start;
} Arena;

typedef struct MyData
{
	char name[5];
	int nums[8];
} MyData;

Arena arena_new(size_t capacity);
void *arena_alloc(Arena *a, size_t size, size_t alignment);
int arena_free(Arena *a);

Arena arena_new(size_t capacity)
{
	void *data = calloc(1, capacity);
	return (Arena) {
		.capacity = capacity,
		.size = 0,
		.data = data,
	};
}

void *arena_alloc(Arena *a, size_t size, size_t alignment)
{
	void *data;

	if (alignment == 0)
	{
		return NULL;
	}

	// Find the padding required for alignment
	// TODO: check if this calculation is the best way to do it.
	size_t padding = (alignment - ((size_t)((uint8_t *)a->data + a->size) % alignment)) % alignment;
	size_t new_size = a->size + padding + size;
	if (new_size > a->capacity)
	{
		return NULL;
	}
	if (size < 1)
	{
		return NULL;
	}

	a->size = new_size;
	data = (uint8_t*)a->data + (size - 1);
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

int arena_reset(Arena *a)
{
	if (a == NULL)
	{
		return 1;
	}
	a->size = 0;
	return 0;
}

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
