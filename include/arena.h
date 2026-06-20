#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <stdint.h>

#define MAX_REGIONS 8

typedef struct Arena_t Arena;

Arena *arena_new(size_t capacity);
void *arena_alloc(Arena *a, size_t size, size_t alignment);
void arena_reset(Arena *a);
void arena_free(Arena *a);

#endif
