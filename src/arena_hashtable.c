#include <arena.h>
#include <arena_string.h>

#include <string.h>

typedef struct HashTableEntry_t
{
	String key;
	uint32_t val;
	struct HashTableEntry_t *next;
} HashTableEntry;

typedef struct HashTable_t
{
	Arena *arena;
	HashTableEntry **entries;
	size_t size;
} HashTable;

#define HASHTABLE_DEFAULT_ALIGNMENT 4

static size_t get_index(uint64_t hash, size_t size)
{
	return (hash % (size - 1)) + 1;
}

HashTable *hashtable_new(size_t size, Arena *a)
{
	HashTable *t = arena_alloc(a, sizeof(HashTable), HASHTABLE_DEFAULT_ALIGNMENT);
	t->arena = a;
	t->size = size;
	t->entries = arena_alloc(a, size * sizeof(HashTableEntry), HASHTABLE_DEFAULT_ALIGNMENT);

	// The 'entry' field must be zero-initialised, so that random data is not mistaken for a valid entry.
	memset(t->entries, 0, size * sizeof(HashTableEntry));

	return t;
}

int hashtable_update(HashTable *t, StringSlice key, uint32_t val)
{
	uint64_t hash = string_slice_hash(key);

	// this should never be zero, so we can use zero as value for 'no entry' in hashtable_get()
	size_t idx = get_index(hash, t->size);

	HashTableEntry *entry = t->entries[idx];
	while (entry != NULL)
	{
		if (string_slice_equal(key, string_as_slice(entry->key)))
		{
			entry->val = val;
			break;
		}
		entry = entry->next;
	}
	entry = arena_alloc(t->arena, sizeof(HashTableEntry), HASHTABLE_DEFAULT_ALIGNMENT);
	if (entry == NULL)
	{
		return -1;
	}
	entry->key = string_new_from_slice(key, t->arena);
	entry->val = val;
	entry->next = NULL;

	t->entries[idx] = entry;
	return 0;
}

uint32_t hashtable_get(HashTable *t, StringSlice key)
{
	uint64_t hash = string_slice_hash(key);
	size_t idx = get_index(hash, t->size);
	HashTableEntry *entry = t->entries[idx];
	while (entry != NULL)
	{
		if (string_slice_equal(key, string_as_slice(entry->key)))
		{
			return entry->val;
		}
	}
	return 0;
}
