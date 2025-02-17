#pragma once

#include <stdint.h>
#include <darray.h>

typedef uint64_t dhash_key_t;

typedef struct{
	dhash_key_t key;
	uint8_t value[];
} dhash_pairing_t;

typedef darray_t dhash_map_t;

extern int dhash_compare(const void *a, const void *b);
extern dhash_key_t dhash_hash(const char *key_base);
extern dhash_map_t *dhash_new(size_t value_size);
extern int dhash_add(dhash_map_t **hash_map, dhash_key_t key, void *value);
extern int dhash_remove(dhash_map_t **hash_map, dhash_key_t);
extern void *dhash_get(dhash_map_t **hash_map, dhash_key_t key);
extern int dhash_set(dhash_map_t **hash_map, dhash_key_t key, void *value);
extern int dhash_delete(dhash_map_t **hash_map);

