#include <stdlib.h>
#include <stdint.h>

#include <arena.h>

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

int arena_reset(Arena *a)
{
	if (a == NULL)
	{
		return 1;
	}
	a->size = 0;
	return 0;
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
