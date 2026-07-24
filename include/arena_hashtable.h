#include <arena.h>
#include <arena_string.h>

typedef struct HashTable_t HashTable;

HashTable *hashtable_new(size_t size, Arena *a);
int hashtable_update(HashTable *t, StringSlice key, uint32_t val);
uint32_t hashtable_get(HashTable *t, StringSlice key);
