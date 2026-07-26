#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>
#include <stdlib.h>

#include <arena.h>

#ifdef __GNUC__

#define MAYBE_UNUSED __attribute__((unused))

#else

#define MAYBE_UNUSED

#endif

#define arraylen(a) array.length

#define DEFINE_ARRAY(name, T)\
	typedef struct name##_t \
	{\
		T *data;\
		size_t length;\
		size_t capacity;\
	} name ;\
	\
	MAYBE_UNUSED static inline name name##_new(size_t capacity, size_t alignment, Arena *arena)\
	{\
		return ( name ) {\
			.capacity = capacity,\
			.length = 0,\
			.data = arena_alloc(arena, sizeof( T ) * capacity, alignment),\
		};\
	}\
	\
	MAYBE_UNUSED static inline int name##_get( name array, size_t idx, T *item)\
	{\
		if (idx >= array.length || idx < 0)\
		{\
			return -1;\
		}\
		*item = array.data[idx];\
		return 0;\
	}\
	\
	MAYBE_UNUSED static inline int name##_push( name *array, T item )\
	{\
		if (array->length >= array->capacity)\
		{\
			return -1;\
		}\
		array->data[array->length++] = item;\
		return 0;\
	}\
	\
	MAYBE_UNUSED static inline int name##_pop( name *array, T *item)\
	{\
		if (array->length <= 0)\
		{\
			return -1;\
		}\
		*item = array->data[--array->length];\
		return 0;\
	}\
	MAYBE_UNUSED static inline int name##_set( name *array, size_t idx, T item)\
	{\
		if (idx >= array->length || idx < 0)\
		{\
			return -1;\
		}\
		array->data[idx] = item;\
		return 0;\
	}\

#endif // ARRAY_H

