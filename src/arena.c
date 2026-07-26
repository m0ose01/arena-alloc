#include <stdlib.h>
#include <stdint.h>

#include <arena.h>
#include <tracy/TracyC.h>

typedef struct Region_t
{
	void *ptr;
	size_t size;
	size_t capacity;
} Region;

struct Arena_t
{
	// TODO: Turn this into a dynamic array.
	Region regions[MAX_REGIONS];
	uint8_t current_region_index;
	uint8_t allocated_regions;
};

static Region region_new(size_t capacity);
static void *region_alloc(Region *r, size_t size, size_t alignment);
static void region_reset(Region *r);
static size_t calculate_padding(const void *ptr, size_t alignment);
static void *arena_malloc_(size_t size);
static void arena_free_(void *ptr);

Arena *arena_new(size_t capacity)
{
	TracyCZone(ctx, 1);
	Arena *arena = arena_malloc_(sizeof(Arena));
	if (arena == NULL)
	{
		return NULL;
	}
	arena->regions[0] = region_new(capacity);
	TracyCZoneEnd(ctx);
	return arena;
}

void *arena_alloc(Arena *a, size_t size, size_t alignment)
{
	TracyCZone(ctx, 1);
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

		size_t padding = calculate_padding((uint8_t *)current_region->ptr + current_region->size, alignment);
		size_t allocation_size = size + padding;
		size_t new_capacity = current_region->capacity > allocation_size ? current_region->capacity : allocation_size;

		if (++a->current_region_index > a->allocated_regions++)
		{
			a->regions[a->current_region_index] = region_new(new_capacity);
		}
		current_region = &(a->regions[a->current_region_index]);

		data = region_alloc(current_region, size, alignment);
	}

	TracyCZoneEnd(ctx);
	return data;
}

void arena_reset(Arena *a)
{
	TracyCZone(ctx, 1);
	for (int current_region_idx = 0; current_region_idx < a->current_region_index + 1; current_region_idx++)
	{
		Region *current_region = &(a->regions[current_region_idx]);
		region_reset(current_region);
	}
	a->current_region_index = 0;
	TracyCZoneEnd(ctx);
}

void arena_free(Arena *a)
{
	TracyCZone(ctx, 1);
	for (int current_region_idx = 0; current_region_idx < a->current_region_index + 1; current_region_idx++)
	{
		Region *current_region = &(a->regions[current_region_idx]);

		arena_free_(current_region->ptr);
		current_region->ptr = NULL;
		current_region->size = 0;
		current_region->capacity = 0;
	}
	a->current_region_index = 0;
	arena_free_(a);
	TracyCZoneEnd(ctx);
}

static void *region_alloc(Region *r, size_t size, size_t alignment)
{
	TracyCZone(ctx, 1)
	size_t padding = calculate_padding((uint8_t *)r->ptr + r->size, alignment);
	size_t new_size = r->size + padding + size;
	if (new_size > r->capacity)
	{
		return NULL;
	}

	void *data = (uint8_t *)r->ptr + r->size + padding;
	r->size = new_size;
	TracyCZoneEnd(ctx);
	return data;
}

static Region region_new(size_t capacity)
{
	TracyCZone(ctx, 1);
	void *data = arena_malloc_(capacity);

	if (data == NULL)
	{
		capacity = 0;
	}

	TracyCZoneEnd(ctx);
	return (Region) {
		.capacity = capacity,
		.size = 0,
		.ptr = data,
	};
}

static void region_reset(Region *r)
{
	TracyCZone(ctx, 1);
	r->size = 0;
	TracyCZoneEnd(ctx);
}

static size_t calculate_padding(const void *ptr, size_t alignment)
{
	return (alignment - ((size_t)((uint8_t *)ptr) % alignment)) % alignment;
}


static void *arena_malloc_(size_t size)
{
	void *ptr = malloc(size);
	TracyCAlloc(ptr, size);
	return ptr;
}

static void arena_free_(void *ptr)
{
	TracyCFree(ptr);
	free(ptr);
}
