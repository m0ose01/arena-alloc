#include <stdlib.h>
#include <stdint.h>

#include <arena.h>

Region region_new(size_t capacity);
void *region_alloc(Region *r, size_t size, size_t alignment);
void region_reset(Region *r);
size_t calculate_padding(const void *ptr, size_t size, size_t alignment);

Arena arena_new(size_t capacity)
{
	return (Arena) {
		.current_region_index = 0,
		.regions = { region_new(capacity) }
	};
}

void *arena_alloc(Arena *a, size_t size, size_t alignment)
{
	if (alignment == 0)
	{
		return NULL;
	}

	if (size < 1)
	{
		return NULL;
	}

	Region *current_region = &(a->regions[a->current_region_index]);

	void *data = region_alloc(current_region, size, alignment);

	// Attempt to realloc once
	if (data == NULL)
	{
		if (a->current_region_index + 1 >= MAX_REGIONS)
		{
			return NULL;
		}

		size_t padding = calculate_padding(current_region->ptr, size, alignment);
		size_t allocation_size = size + padding;
		size_t new_capacity = current_region->capacity > allocation_size ? current_region->capacity : allocation_size;

		a->regions[++a->current_region_index] = region_new(new_capacity);
		current_region = &(a->regions[a->current_region_index]);

		data = region_alloc(current_region, size, alignment);
	}

	return data;
}

void arena_reset(Arena *a)
{
	a->current_region_index = 0;

	Region *current_region = &(a->regions[a->current_region_index]);
	region_reset(current_region);
}

void arena_free(Arena *a)
{
	for (int current_region_idx = 0; current_region_idx < a->current_region_index + 1; current_region_idx++)
	{
		Region *current_region = &(a->regions[current_region_idx]);

		free(current_region->ptr);
		current_region->ptr = NULL;
		current_region->size = 0;
		current_region->capacity = 0;
	}
	a->current_region_index = 0;
}

void *region_alloc(Region *r, size_t size, size_t alignment)
{
	size_t padding = calculate_padding(r->ptr, size, alignment);
	size_t new_size = r->size + padding + size;
	if (new_size > r->capacity)
	{
		return NULL;
	}

	void *data = (uint8_t*)r->ptr + r->size;
	r->size = new_size;
	return data;
}

Region region_new(size_t capacity)
{
	void *data = calloc(1, capacity);

	if (data == NULL)
	{
		capacity = 0;
	}

	return (Region) {
		.capacity = capacity,
		.size = 0,
		.ptr = data,
	};
}

void region_reset(Region *r)
{
	r->size = 0;
}

size_t calculate_padding(const void *ptr, size_t size, size_t alignment)
{
	return (alignment - ((size_t)((uint8_t *)ptr + size) % alignment)) % alignment;
}

