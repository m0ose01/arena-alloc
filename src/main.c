#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <arena.h>
#include <arena_string.h>

typedef struct MyData
{
	char name[5];
	int nums[8];
} MyData;

Arena *context_arena = NULL;
const size_t default_alignment = 8;

void *context_alloc(size_t size)
{
	return arena_alloc(context_arena, size, default_alignment);
}

void context_free(void *ptr)
{
	(void)ptr;
	return;
}

int main(void)
{
	Arena *my_memory = arena_new(sizeof(MyData) * 2);

	MyData *my_data = arena_alloc(my_memory, sizeof(MyData), sizeof(long long));
	if (my_data == NULL)
	{
		printf("Could not allocate from arena\n");
		return 1;
	}
	MyData *my_data_2 = arena_alloc(my_memory, sizeof(MyData), sizeof(long long));
	if (my_data_2 == NULL)
	{
		printf("Could not allocate from arena\n");
		return 1;
	}
	char *hi = "hiii";
	for (int ii = 0; ii < 5; ii++)
	{
		my_data->name[ii] = hi[ii];
		my_data_2->name[ii] = hi[ii];
	}
	for (int ii = 0; ii < 8; ii++)
	{
		my_data->nums[ii] = (8 - 1) - ii;
		my_data_2->nums[ii] = ii;
	}

	printf("%i\n", my_data->nums[7]);
	printf("%s\n", my_data_2->name);

	arena_reset(my_memory);

	char *my_new_data = arena_alloc(my_memory, sizeof(char) * 8, sizeof(long long));
	for (int ii = 0; ii < 8; ii++)
	{
		my_new_data[ii] = 'A' + ii;
	}
	my_new_data[8] = '\0';
	printf("%s\n", my_new_data);

	Arena *large_arena = arena_new(sizeof(char) * (26 + 1) * 15);

	char *str;
	for (int ii = 0; ii < 100; ii++)
	{
		arena_reset(large_arena);
		const int count = sizeof(char) * (26);
		str = arena_alloc(large_arena, count, sizeof(char));
		if (str == NULL)
		{
			fprintf(stderr, "Error: could not allocate from arena.\n");
			break;
		}
		for (int jj = 0; jj < count; jj++)
		{
			str[jj] = 'A' + (jj % 26);
		}
		str[count - 1] = '\0';
	}

	arena_free(my_memory);
	arena_free(large_arena);

	context_arena = arena_new(8192);

	string_set_alloc_function(context_alloc);

	String text = STRING_FROM_STATIC_STRING("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin mollis nulla ut justo efficitur, interdum pretium felis congue. Morbi et elit eu eros faucibus pulvinar sit amet in dolor. Sed feugiat, odio ut viverra consectetur, magna ante laoreet enim, non pulvinar est dui id diam. Nulla at mi laoreet, tempus enim a, ultricies purus. Nam sollicitudin est a enim euismod sollicitudin. Donec tristique mauris id erat gravida, a congue ligula pellentesque. Aenean in aliquet tellus. Nunc ac facilisis erat. Vivamus mollis hendrerit pulvinar. Vestibulum ante nunc, mattis vel purus nec, fringilla ullamcorper lorem. Vestibulum in ex a velit congue tristique. Duis quis accumsan mauris. Curabitur non varius quam, non rutrum urna. Mauris a mi et erat dictum vehicula non vulputate nunc. Duis aliquam diam sit amet urna rutrum, ac ultrices neque commodo. In tincidunt suscipit lacus. Suspendisse eu gravida arcu. Duis posuere, purus non tempor blandit, mi metus placerat sem, at lacinia velit ante vel est. Donec mi nulla, malesuada id enim vehicula, semper volutpat risus. Praesent congue, ipsum eget imperdiet tincidunt, libero libero aliquet velit, vitae cursus dui massa non eros. Donec eu sollicitudin mi. Nullam nec sagittis libero. Maecenas a ex neque. Curabitur ac vehicula risus. Mauris id orci eget ante bibendum tempus. Ut dictum auctor odio, ac consectetur purus porttitor at. Sed sed lacus non quam egestas semper sit amet a nibh. Nam tempor justo eu ex tempus tempor. Aliquam vehicula ante sit amet mi rutrum, non vehicula ligula posuere. Duis pretium consectetur vehicula. Aliquam vestibulum porttitor elementum. Vestibulum molestie vel augue sit amet pellentesque. Integer vitae sapien ac sapien interdum ornare sit amet eu enim. Nullam lacinia tellus sed augue convallis ultricies. Integer vitae massa ac lectus porta efficitur vitae ut nulla. Vivamus non pharetra dolor. Aenean rutrum urna at egestas varius. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nunc tempus non nisi eu imperdiet. Etiam ullamcorper, lacus eu convallis finibus, est odio laoreet nulla, malesuada consectetur ex elit in neque. Vestibulum sollicitudin augue sit amet ipsum vestibulum iaculis. Cras venenatis semper ultricies. Aliquam erat volutpat. Duis quis tortor eget augue pharetra varius. Nulla imperdiet tempus ipsum id tristique. Nullam imperdiet condimentum lacus quis porta. Ut ut imperdiet mauris. Aliquam erat volutpat. In mattis auctor scelerisque. Cras vel molestie augue, ac dapibus metus. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. In maximus eros bibendum risus lacinia, sed rutrum ipsum porta. Vivamus faucibus porttitor tortor, quis sagittis sapien vestibulum in. Aenean a eros non velit cursus porta.");

	char last_char = 'b';
	int success = string_pop(&text, &last_char);
	if (success != 0)
	{
		fprintf(stderr, "Error: could not pop char\n");
		return -1;
	}

	fprintf(stdout, "%.*s\n", STRING_FORMAT(text));
	fprintf(stdout, "%s\n", string_to_cstring(text));

	StringSlice a, b;
	string_split(text, SLICE_FROM_STATIC_STRING("sit amet"), &a, &b);

	fprintf(stdout, "%.*s\n%.*s\n", STRING_FORMAT(a), STRING_FORMAT(b));

	String new = string_copy(text);
	fprintf(stdout, "Strings are Equal: %s.\n", string_slice_equal(string_slice(new, 0, 18), a) ? "true": "false");

	arena_free(context_arena);
	context_arena = NULL;
}
