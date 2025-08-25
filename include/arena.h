#include <stdlib.h>

typedef struct Arena
{
	size_t capacity;
	size_t size;
	void *data;
	void *start;
} Arena;

Arena arena_new(size_t capacity);
void *arena_alloc(Arena *a, size_t size, size_t alignment);
int arena_reset(Arena *a);
int arena_free(Arena *a);
