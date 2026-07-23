#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <arena.h>
#include <arena_string.h>

AllocFunction string_default_alloc = malloc;

String string_new(size_t capacity)
{
	String string = {
		.capacity = capacity,
		.length = 0,
		.data = string_default_alloc(sizeof( char ) * (capacity + 1))
	};
	string.data[capacity] = '\0';
	return string;
}

String string_new_from_slice(StringSlice slice)
{
	size_t capacity = sizeof(char) * slice.length;
	String string = {
		.data = string_default_alloc(capacity + 1),
		.capacity = capacity / sizeof(char),
		.length = slice.length,
	};

	for (size_t current_char = 0; current_char < string.length; current_char++)
	{
		string.data[current_char] = slice.data[current_char];
	}
	string.data[capacity] = '\0';

	return string;
}

char *string_to_cstring(String str)
{
	char *cstring = string_default_alloc(sizeof(char) * (str.length + 1));
	for (size_t current_char = 0; current_char < str.length; current_char++)
	{
		cstring[current_char] = str.data[current_char];
	}
	cstring[str.length] = '\0';
	return cstring;
}

StringSlice string_slice(String str, size_t begin, size_t length)
{
	return (StringSlice) {
		.data = str.data + begin,
		.length = length,
	};
}

StringSlice string_as_slice(String str)
{
	return (StringSlice) {
		.data = str.data,
		.length = str.length,
	};
}

String string_new_arena(size_t capacity, size_t alignment, Arena *arena)
{
	char *data = arena_alloc(arena, sizeof( char ) * (capacity + 1), alignment);
	data[capacity] = '\0';
	return ( String ) {
		.capacity = capacity,
		.length = 0,
		.data = data,
	};
}

int string_get(String str, size_t idx, char *item)
{
	if (idx >= str.length || idx < 0)
	{
		return -1;
	}
	*item = str.data[idx];
	return 0;
}

int string_push(String *str, char item)
{
	if (str->length >= (str->capacity - 1))
	{
		return -1;
	}
	str->data[str->length++] = item;
	return 0;
}

int string_pop(String *str, char *item)
{
	if (str->length <= 0)
	{
		return -1;
	}
	*item = str->data[--str->length];
	return 0;
}

int string_set(String *str, size_t idx, char item)
{
	if (idx >= str->length || idx < 0)
	{
		return -1;
	}
	str->data[idx] = item;
	return 0;
}

void string_set_alloc_function(AllocFunction f)
{
	string_default_alloc = f;
}

int string_trim(String *str, size_t n)
{
	if (str->length < n)
	{
		return -1;
	}
	str->length -= n;
	return 0;
}

bool string_slice_equal(StringSlice slice_a, StringSlice slice_b)
{
	if (slice_a.length != slice_b.length)
	{
		return false;
	}

	for (size_t current_char = 0; current_char < slice_a.length; current_char++)
	{
		if (slice_a.data[current_char] != slice_b.data[current_char])
		{
			return false;
		}
	}
	return true;
}

size_t string_find(String str, StringSlice pattern)
{
	for (size_t current_char_idx = 0; current_char_idx < str.length; current_char_idx++)
	{
		StringSlice substring = string_slice(str, current_char_idx, pattern.length);
		if (string_slice_equal(substring, pattern))
		{
			return current_char_idx;
		}
	}
	return 0;
}

size_t string_split(String str, StringSlice pattern, StringSlice *slice_a, StringSlice *slice_b)
{
	size_t index = string_find(str, pattern);
	if (index == 0)
	{
		*slice_a = string_slice(str, 0, 0);
		*slice_b = string_slice(str, 0, 0);
		return index;
	}
	*slice_a = string_slice(str, 0, index);
	size_t slice_b_start = index + pattern.length;
	*slice_b = string_slice(str, slice_b_start, str.length - slice_b_start);
	return index;
}

String string_copy(String str)
{
	return string_new_from_slice(string_slice(str, 0, str.length));
}

uint64_t string_hash(StringSlice slice)
{
	const uint64_t prime_numbers[] = { 55476315010378721, 63646009107264737, 67354137784070743, 39967243858431091, 57601086460062329, 39793605331455817, 58692382681956689, 52497690708979949 };
	const uint_fast8_t n_prime_numbers = sizeof(prime_numbers) / sizeof(uint64_t);

	uint64_t hash = 1;
	for (size_t current_char = 0; current_char < slice.length; current_char++)
	{
		hash *= prime_numbers[slice.data[current_char] % n_prime_numbers] + prime_numbers[(slice.data[current_char] + 1) % n_prime_numbers] ^ prime_numbers[(slice.data[current_char]+ 2) % n_prime_numbers];
	}
	return hash;
}
