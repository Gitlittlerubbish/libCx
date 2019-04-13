#ifndef __libcx_hashmap_algos_h__
#define __libcx_hashmap_algos_h__

#include <stdint.h>

uint32_t Hashmap_fnv1a_hash(void *data);

uint32_t Hashmap_adler32_hash(void *data);

uint32_t Hashmap_dbj_hash(void *data);


#endif