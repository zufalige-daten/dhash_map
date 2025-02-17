#include <dhash_map.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <darray.h>

int dhash_compare(const void *a, const void *b){
	return (int)(((dhash_pairing_t *)a)->key - ((dhash_pairing_t *)b)->key);
}

dhash_key_t dhash_hash(const char *key_base){
	dhash_key_t hash;
	hash = 0x811c9dc5;

	while(*key_base)
		hash = (((dhash_key_t)(*(key_base++))) ^ hash) * 0x01000193;

	return hash;
}

dhash_map_t *dhash_new(size_t value_size){
	dhash_map_t *ret = (dhash_map_t *)darray_new(sizeof(dhash_pairing_t) + value_size);
	return ret;
}

int dhash_add(dhash_map_t **hash_map, dhash_key_t key, void *value){
	uintptr_t basic, index;
	int status;

	if(*hash_map == NULL){
		return 1;
	}

	basic = (uintptr_t)bsearch(&key, (*hash_map)->array, (*hash_map)->length, (*hash_map)->unit, dhash_compare);

	if(basic == NULL){
		index = 0; // TODO: implement binary search to search for index insersion
	}
}

int dhash_remove(dhash_map_t **hash_map, dhash_key_t){
}

void *dhash_get(dhash_map_t **hash_map, dhash_key_t key){
}

int dhash_set(dhash_map_t **hash_map, dhash_key_t key, void *value){
}

int dhash_delete(dhash_map_t **hash_map){
}

