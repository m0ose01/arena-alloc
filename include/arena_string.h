#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <arena.h>

typedef struct string_t
{
	char *data;
	size_t length;
	size_t capacity;
} String;

typedef struct string_slice_t
{
	char *data;
	size_t length;
} StringSlice;

typedef void*(*AllocFunction)(size_t size);
typedef void(*FreeFunction)(void *);

String string_new(size_t capacity);
String string_new_from_slice(StringSlice slice);
char *string_to_cstring(String str);
StringSlice string_slice(String str, size_t begin, size_t length);
StringSlice string_as_slice(String str);
String string_new_arena(size_t capacity, size_t alignment, Arena *arena);
int string_push(String *str, char item);
int string_pop(String *str, char *item);
int string_set(String *array, size_t idx, char item);
void string_set_alloc_function(AllocFunction f);
size_t string_find(String str, StringSlice pattern);
size_t string_split(String str, StringSlice pattern, StringSlice *slice_a, StringSlice *slice_b);
String string_copy(String str);
int string_trim(String *str, size_t n);
bool string_slice_equal(StringSlice slice_a, StringSlice slice_b);
uint64_t string_hash(StringSlice slice);

#define STRING_FORMAT(s) (int)s.length, s.data
#define STRING_LENGTH(s) s.length
#define SLICE_FROM_STATIC_STRING(s) (StringSlice){ .data = s, .length = sizeof(s) - 1 }
#define SLICE_FROM_STATIC_STRING_AND_LENGTH(s, l) (StringSlice){ .data = s, .length = l }
#define STRING_FROM_STATIC_STRING(s) string_new_from_slice( (StringSlice){ .data = s, .length = sizeof(s) - 1 } )
#define STRING_FROM_STATIC_STRING_AND_LENGTH(s, l) string_new_from_slice( (StringSlice){ .data = s, .length = l } )
#define STRING_GET_RAW_POINTER(s) s.data

#endif
