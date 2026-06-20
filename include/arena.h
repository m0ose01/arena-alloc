#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <stdint.h>

#define MAX_REGIONS 8

typedef struct Region_t
{
	void *ptr;
	size_t size;
	size_t capacity;
} Region;

typedef struct Arena_t
{
	// TODO: Turn this into a dynamic array.
	Region regions[MAX_REGIONS];
	uint8_t current_region_index;
} Arena;

Arena arena_new(size_t capacity);
void *arena_alloc(Arena *a, size_t size, size_t alignment);
void arena_reset(Arena *a);
void arena_free(Arena *a);

#endif
