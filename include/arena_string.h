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

String string_new(size_t capacity, Arena *a);
String string_new_from_slice(StringSlice slice, Arena *a);
String string_copy(String str, Arena *a);
char *string_slice_to_cstring(StringSlice slice, Arena *a);

StringSlice string_slice(String str, size_t begin, size_t length);
StringSlice string_as_slice(String str);
int string_push(String *str, char item);
int string_pop(String *str, char *item);
int string_set(String *array, size_t idx, char item);
size_t string_find(String str, StringSlice pattern);
size_t string_split(String str, StringSlice pattern, StringSlice *slice_a, StringSlice *slice_b);
int string_trim(String *str, size_t n);
bool string_slice_equal(StringSlice slice_a, StringSlice slice_b);
uint64_t string_slice_hash(StringSlice slice);

#define STRING_FORMAT(s) (int)s.length, s.data
#define STRING_LENGTH(s) s.length
#define SLICE_FROM_STATIC_STRING(s) (StringSlice){ .data = s, .length = sizeof(s) - 1 }
#define SLICE_FROM_STATIC_STRING_AND_LENGTH(s, l) (StringSlice){ .data = s, .length = l }
#define STRING_GET_RAW_POINTER(s) s.data

#ifdef __GNUC__

#define MAYBE_UNUSED __attribute__((unused))

#else

#define MAYBE_UNUSED

#endif

#define DEFINE_STRING_ALLOCATOR_FUNCTIONS(prefix, arena)\
	MAYBE_UNUSED static inline String prefix ## _string_new(size_t capacity) { return string_new(capacity, arena); }\
	MAYBE_UNUSED static inline String prefix ## _string_new_from_slice(StringSlice slice) { return string_new_from_slice(slice, arena); }\
	MAYBE_UNUSED static inline String prefix ## _string_copy(String str) { return string_copy(str, arena); }\
	MAYBE_UNUSED static inline char* prefix ## _string_slice_to_cstring(StringSlice slice) { return string_slice_to_cstring(slice, arena); }\

#endif
