#include "radixmap.h"
#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//get the offset--y of x
#define ByteOf(x, y) (((uint8_t *)(x))[(y)]) 

//create a map
RadixMap *RadixMap_create(size_t max) {
	RadixMap *map = calloc(1, sizeof(RadixMap));
	check_mem(map);

	map->contents = calloc(max + 1, sizeof(RMElement));
	check_mem(map->contents);

	map->temp = calloc(max + 1, sizeof(RMElement));
	check_mem(map->temp);

	check(max > 0, "Max must be positive.");

	map->max = max;
	map->end = 0;

	return map;
error:
	RadixMap_destroy(map);
	return NULL;
}

//free pointers
void RadixMap_destroy(RadixMap *map) {
	if (map) {
		if (map->contents)
			free(map->contents);
		if (map->temp)
			free(map->temp);
		free(map);
	}
}

//call this function with each 'digit'
static inline void radix_sort(short offset, uint64_t max,
					uint64_t *source, uint64_t *dest) {
	uint64_t buckets[256] = { 0 };
	uint64_t *bp = NULL;
	uint64_t *sp = NULL;
	uint64_t *end = NULL;
	uint64_t s = 0;
	uint64_t b = 0;

	for (sp = source, end = source + max; sp < end; sp++) {
		buckets[ByteOf(sp, offset)]++;
	}

	for (s = 0, bp = buckets, end = buckets + 256; bp < end; bp++) {
		b = *bp;
		*bp = s;
		s += b;
	}

	for (sp = source, end = source + max; sp < end; sp++) {
		bp = buckets + ByteOf(sp, offset);
		dest[*bp] = *sp;
		(*bp)++;
	}
}

//sort 4 'digit' of unsigned int 32-->key.
void RadixMap_sort(RadixMap *map) {
	uint64_t *source = &map->contents[0].raw;
	uint64_t *dest = &map->temp[0].raw;

	radix_sort(0, map->end, source, dest);
	radix_sort(1, map->end, dest, source);
	radix_sort(2, map->end, source, dest);
	radix_sort(3, map->end, dest, source);
}

//find key matched RMElement
RMElement *RadixMap_find(RadixMap *map, uint32_t to_find) {
	int low = 0;
	int high = map->end - 1;
	RMElement *data = map->contents;

	while (low <= high) {
		int middle = low + (high - low) / 2;
		uint32_t key = data[middle].data.key;

		if (to_find < key) {
			high = middle - 1;
		} else if (to_find > key) {
			low = middle + 1;
		} else {
			return &data[middle];
		}
	}

	return NULL;
}

//add a RMElement and the make the map sorted 
int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value) {
	check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");
	
	RMElement ele = { .data = { .key = key,.value = value} };
	check(map->end + 1 < map->max, "RadixMap is full.");

	map->contents[map->end++] = ele;

	RadixMap_sort(map);

	return 0;
error:
	return -1;
}

//delete an element by set key 0xFFFFFFFF, and make the map sorted
int RadixMap_delete(RadixMap *map, RMElement *ele) {
	check(map->end > 0, "There is nothing to delete.");
	check(ele != NULL, "Cannot delete a NULL element.");

	ele->data.key = UINT32_MAX;

	if (map->end > 1) {
		RadixMap_sort(map);
	}

	map->end--;

	return 0;
error:
	return -1;
}









