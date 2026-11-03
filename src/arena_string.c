#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <tracy/TracyC.h>

#include <arena.h>
#include <arena_string.h>

static size_t get_formatted_string_length(StringSlice fmt, va_list v);

String string_new(size_t capacity, Arena *a)
{
	TracyCZone(ctx, true);
	char *data = arena_alloc(a, sizeof( char ) * (capacity + 1), 1);
	data[capacity] = '\0';
	TracyCZoneEnd(ctx);
	return ( String ) {
		.capacity = capacity,
		.length = 0,
		.data = data,
	};
}

String string_new_from_slice(StringSlice slice, Arena *a)
{
	TracyCZone(ctx, true);
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

	TracyCZoneEnd(ctx);
	return string;
}

String string_from_cstring(char *cstring, Arena *a)
{
	TracyCZone(ctx, true);
	if (cstring == NULL)
	{
		return (String) { .data = NULL, .length = 0, .capacity = 0 };
	}

	const size_t max_c_string_length = 0;
	size_t length = 0;

	for (size_t current_char = 0; (current_char < max_c_string_length) || (max_c_string_length == 0); current_char++)
	{
		if (cstring[current_char] == '\0')
		{
			break;
		}
		length++;
	}

	StringSlice cstring_slice = SLICE_FROM_PTR_AND_LENGTH(cstring, length);

	TracyCZoneEnd(ctx);
	return string_new_from_slice(cstring_slice, a);
}

String string_format(StringSlice format_string, Arena *a, ...)
{
	TracyCZone(ctx, true);

	va_list args;
	va_start(args, a);

	String new_string = string_format_v(format_string, a, args);

	va_end(args);

	TracyCZoneEnd(ctx);
	return new_string;
}

String string_format_v(StringSlice format_string, Arena *a, va_list args)
{
	TracyCZone(ctx, true);

	va_list args_b;
	va_copy(args_b, args);

	size_t formatted_string_length = get_formatted_string_length(format_string, args_b);

	String new_string = string_new(formatted_string_length, a);

	String empty_string = {
		.data = NULL,
		.length = 0,
		.capacity = 0,
	};

	for (size_t current_char_idx = 0; current_char_idx < STRING_LENGTH(format_string); current_char_idx++)
	{
		char current_char = format_string.data[current_char_idx];
		if (current_char == '\0')
		{
			break;
		}

		if (current_char != '%')
		{
			if (string_push(&new_string, current_char) != 0)
			{
				va_end(args_b);
				TracyCZoneEnd(ctx);
				return empty_string;
			}
			continue;
		}
		current_char_idx++;
		if (current_char_idx >= STRING_LENGTH(format_string))
		{
			break;
		}
		current_char = format_string.data[current_char_idx];

		switch (current_char)
		{
			case 's':
				{
					StringSlice slice = va_arg(args, StringSlice);
					if (string_push_slice(&new_string, slice) != 0)
					{
						va_end(args_b);
						TracyCZoneEnd(ctx);
						return empty_string;
					}
					break;
				}
			default:
				// invalid format string, return empty string
				va_end(args_b);
				TracyCZoneEnd(ctx);
				return empty_string;
		}

	}
	va_end(args_b);

	TracyCZoneEnd(ctx);
	return new_string;
}

char *string_slice_to_cstring(StringSlice slice, Arena *a)
{
	TracyCZone(ctx, true);
	char *cstring = arena_alloc(a, sizeof(char) * (slice.length + 1), 1);
	for (size_t current_char = 0; current_char < slice.length; current_char++)
	{
		cstring[current_char] = slice.data[current_char];
	}
	cstring[slice.length] = '\0';
	TracyCZoneEnd(ctx);
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
	TracyCZone(ctx, true);
	if (idx >= str.length)
	{
		TracyCZoneEnd(ctx);
		return -1;
	}
	*item = str.data[idx];
	TracyCZoneEnd(ctx);
	return 0;
}

int string_push(String *str, char item)
{
	TracyCZone(ctx, true);
	if (str->length >= (str->capacity))
	{
		TracyCZoneEnd(ctx);
		return -1;
	}
	str->data[str->length++] = item;
	TracyCZoneEnd(ctx);
	return 0;
}

int string_push_slice(String *str, StringSlice slice)
{
	TracyCZone(ctx, true);
	size_t new_length = STRING_LENGTH((*str)) + STRING_LENGTH(slice);
	if (new_length > str->capacity)
	{
		return -1;
	}

	for (size_t current_char_idx = 0; current_char_idx < STRING_LENGTH(slice); current_char_idx++)
	{
		char current_char = slice.data[current_char_idx];
		if (string_push(str, current_char) != 0)
		{
			return -1;
		}
	}

	TracyCZoneEnd(ctx);
	return 0;
}

int string_pop(String *str, char *item)
{
	TracyCZone(ctx, true);
	if (str->length == 0)
	{
		TracyCZoneEnd(ctx);
		return -1;
	}
	*item = str->data[--str->length];
	TracyCZoneEnd(ctx);
	return 0;
}

int string_set(String *str, size_t idx, char item)
{
	TracyCZone(ctx, true);
	if (idx >= str->length)
	{
		TracyCZoneEnd(ctx);
		return -1;
	}
	str->data[idx] = item;
	TracyCZoneEnd(ctx);
	return 0;
}

int string_trim(String *str, size_t n)
{
	TracyCZone(ctx, true);
	if (str->length < n)
	{
		TracyCZoneEnd(ctx);
		return -1;
	}
	str->length -= n;
	TracyCZoneEnd(ctx);
	return 0;
}

int natoi(char *s, int n)
{
    int x = 0;
    while(isdigit(s[0]) && n--)
    {
        x = x * 10 + (s[0] - '0');      
        s++;
    }
    return x;
}

int string_slice_to_uint64(StringSlice slice, uint64_t *out)
{
	int success = 0;
	uint64_t number = 0;
	for (size_t current_char = 0; current_char < slice.length; current_char++)
	{
		if (!isdigit((unsigned char)slice.data[current_char]))
		{
			success = -1;
			break;
		}
		number = number * 10 + (slice.data[current_char] - '0');
	}

	*out = number;
	return success;
}

bool string_slice_equal(StringSlice slice_a, StringSlice slice_b)
{
	TracyCZone(ctx, true);
	if (slice_a.length != slice_b.length)
	{
		TracyCZoneEnd(ctx);
		return false;
	}

	for (size_t current_char = 0; current_char < slice_a.length; current_char++)
	{
		if (slice_a.data[current_char] != slice_b.data[current_char])
		{
			TracyCZoneEnd(ctx);
			return false;
		}
	}
	TracyCZoneEnd(ctx);
	return true;
}

size_t string_find(String str, StringSlice pattern)
{
	TracyCZone(ctx, true);
	if (str.length < pattern.length)
	{
		TracyCZoneEnd(ctx);
		return 0;
	}

	for (size_t current_char_idx = 0; current_char_idx < (str.length - pattern.length + 1); current_char_idx++)
	{
		StringSlice substring = string_slice(str, current_char_idx, pattern.length);
		if (string_slice_equal(substring, pattern))
		{
			TracyCZoneEnd(ctx);
			return current_char_idx;
		}
	}
	TracyCZoneEnd(ctx);
	return 0;
}

size_t string_split(String str, StringSlice pattern, StringSlice *slice_a, StringSlice *slice_b)
{
	TracyCZone(ctx, true);
	size_t index = string_find(str, pattern);
	if (index == 0)
	{
		*slice_a = string_slice(str, 0, 0);
		*slice_b = string_slice(str, 0, 0);
		TracyCZoneEnd(ctx);
		return index;
	}
	*slice_a = string_slice(str, 0, index);
	size_t slice_b_start = index + pattern.length;
	*slice_b = string_slice(str, slice_b_start, str.length - slice_b_start);
	TracyCZoneEnd(ctx);
	return index;
}

String string_copy(String str, Arena *a)
{
	return string_new_from_slice(string_slice(str, 0, str.length), a);
}

uint64_t string_slice_hash(StringSlice slice)
{
	TracyCZone(ctx, true);
	const uint64_t prime_numbers[] = { 55476315010378721, 63646009107264737, 67354137784070743, 39967243858431091, 57601086460062329, 39793605331455817, 58692382681956689, 52497690708979949 };
	const uint_fast8_t n_prime_numbers = sizeof(prime_numbers) / sizeof(uint64_t);

	uint64_t hash = 1;
	for (size_t current_char = 0; current_char < slice.length; current_char++)
	{
		hash *= (prime_numbers[(unsigned char)slice.data[current_char] % n_prime_numbers] + prime_numbers[((unsigned char)slice.data[current_char] + 1) % n_prime_numbers]) ^ (prime_numbers[((unsigned char)slice.data[current_char]+ 2) % n_prime_numbers]);
	}
	TracyCZoneEnd(ctx);
	return hash;
}


// Internal Functions

static size_t get_formatted_string_length(StringSlice format_string, va_list args)
{
	size_t new_length = 0;

	for (size_t current_char_idx = 0; current_char_idx < STRING_LENGTH(format_string); current_char_idx++)
	{
		char current_char = format_string.data[current_char_idx];
		if (current_char == '\0')
		{
			break;
		}

		if (current_char != '%')
		{
			new_length++;
			continue;
		}
		current_char_idx++;
		if (current_char_idx >= STRING_LENGTH(format_string))
		{
			break;
		}
		current_char = format_string.data[current_char_idx];

		switch (current_char)
		{
			case 's':
				{
					StringSlice slice = va_arg(args, StringSlice);
					new_length += STRING_LENGTH(slice);
					break;
				}
			default:
				// invalid format string, return empty string
				return 0;
		}

	}

	return new_length;
}
