#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <arena.h>
#include <arena_string.h>

String string_new(size_t capacity, Arena *a)
{
	char *data = arena_alloc(a, sizeof( char ) * (capacity + 1), 1);
	data[capacity] = '\0';
	return ( String ) {
		.capacity = capacity,
		.length = 0,
		.data = data,
	};
}

String string_new_from_slice(StringSlice slice, Arena *a)
{
	size_t capacity = sizeof(char) * slice.length;
	String string = {
		.data = arena_alloc(a, capacity + 1, 1),
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

char *string_slice_to_cstring(StringSlice slice, Arena *a)
{
	char *cstring = arena_alloc(a, sizeof(char) * (slice.length + 1), 1);
	for (size_t current_char = 0; current_char < slice.length; current_char++)
	{
		cstring[current_char] = slice.data[current_char];
	}
	cstring[slice.length] = '\0';
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

String string_copy(String str, Arena *a)
{
	return string_new_from_slice(string_slice(str, 0, str.length), a);
}

uint64_t string_slice_hash(StringSlice slice)
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
