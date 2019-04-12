#undef NDEBUG
#include <stdint.h>
#include "hashmap.h"
#include "dbg.h"
#include "bstrlib.h"

static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring) a, (bstring) b);
}

/** 
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t default_hash(void *a)
{
    size_t len = blength((bstring) a);
    char *key = bdata((bstring) a);
    uint32_t hash = 0;
    uint32_t i = 0;

    for (hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

//similar to hash_map, using DArray
Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
	Hashmap *map = calloc(1, sizeof(Hashmap));
	check_mem(map);
	map->compare = compare == NULL ? default_compare : compare;
	map->hash = hash == NULL ? default_hash : hash;

	map->buckets = DArray_create(sizeof(DArray), DEFAULT_NUMBER_OF_BUCKETS); //chaining, using DArray
	check_mem(map->buckets);
	map->buckets->end = map->buckets->max; //no more expansion

	return map;
error:
	Hashmap_destroy(map);
	return NULL;

}
	
//free works
void Hashmap_destroy(Hashmap *map)
{
	int i = 0;
	if (map) {
		if (map->buckets) {
			for (i = 0; i < DArray_count(map->buckets); i++) {
				DArray *bucket = DArray_get(map->buckets, i);
				DArray_clear_destroy(bucket);
			}

			DArray_destroy(map->buckets);
		}

		free(map);
	}
}

//create a HashmapNode
static inline HashmapNode *Hashmap_node_create(uint32_t hash, void *key, void *data)
{
	HashmapNode *node = calloc(1, sizeof(HashmapNode));
	check_mem(node);

	node->key = key;
	node->data = data;
	node->hash = hash;

    return node;

error:
    return NULL;
}

//get & set both use this function, controlled with create(0/1).
static inline DArray *Hashmap_find_bucket(Hashmap *map, void *key, int create, uint32_t *hash)
{
	*hash = map->hash(key);
	int bucket_n = *hash % DEFAULT_NUMBER_OF_BUCKETS;
	check(bucket_n >= 0, "Invalid bucket_n");

    DArray *bucket = DArray_get(map->buckets, bucket_n);

    if (!bucket && create) {
        // new bucket, set it up
        bucket = DArray_create(sizeof(HashmapNode), DEFAULT_NUMBER_OF_BUCKETS); //changed to sizeof(HashmapNode)
        check_mem(bucket);

        DArray_set(map->buckets, bucket_n, bucket);
    }

    return bucket;

error:
    return NULL;
}

//find the bucket and then add the new HashmapNode
int Hashmap_set(Hashmap *map, void *key, void *data)
{
	uint32_t hash = 0;
	DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
	check(bucket, "ERRROR, can't create bucket");

	HashmapNode *node = Hashmap_node_create(hash, key, data);
	check_mem(node);

	DArray_push(bucket, node);

    return 0;

error:
    return -1;
}

//inner function: return the index of the HashmapNode
static inline int Hashmap_get_node(Hashmap *map, uint32_t hash, DArray *bucket, void *key)
{
    int i = 0;

    for (i = 0; i < DArray_end(bucket); i++) {
        debug("TRY: %d", i);
        HashmapNode *node = DArray_get(bucket, i);
        if (node->hash == hash && map->compare(node->key, key) == 0) {
            return i;
        }
    }

    return -1;
}

//get the data of the HashmapNode
void *Hashmap_get(Hashmap *map, void *key)
{
	uint32_t hash = 0;
	DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);

	int index = Hashmap_get_node(map, hash, bucket, key);
	if (index == -1) 
		return NULL;

	HashmapNode *node = DArray_get(bucket, index);
	check(node, "Get HashmapNde error.");

    return node->data;

error:			// fallthrough
    return NULL;
}

//traverse Hashmap
int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb)
{
    int i = 0;
    int j = 0;
    int rc = 0;

    for (i = 0; i < DArray_count(map->buckets); i++) {
        DArray *bucket = DArray_get(map->buckets, i);
        if (bucket) {
            for (j = 0; j < DArray_count(bucket); j++) {
                HashmapNode *node = DArray_get(bucket, j);
                rc = traverse_cb(node);
                if (rc != 0)
                    return rc;
            } 
        }
    }

    return 0;
}

//free the HashmapNode, and return the data.
void *Hashmap_delete(Hashmap *map, void *key)
{
	uint32_t hash = 0;
	DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
	if (bucket == NULL) 
		return NULL;

	int index = Hashmap_get_node(map, hash, bucket, key);
	if (index == -1)
		return NULL;

	HashmapNode *node = DArray_get(bucket, index);
	if (node == NULL) 
		return NULL;

	void *data = node->data;
	free(node);

	//if node is the last, then it is already freed.
	HashmapNode *last_node = DArray_pop(bucket); 

	//get the DArray pushed.
	if (last_node != node) {
		DArray_set(bucket, index, last_node);
	}

    return data;
}

