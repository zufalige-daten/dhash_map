#pragma once

#include <stdint.h>
#include <darray.h>

typedef uint64_t dhash_key_t;

typedef struct{
	dhash_key_t key;
	uint8_t value[];
} dhash_pairing_t;

typedef struct{
	darray_t *darray;
	size_t storage_unit;
} dhash_map_t;

#define _alwaysinline inline __attribute__((always_inline))
_alwaysinline dhash_key_t _dhash_hash(const char *key_base){
	dhash_key_t hash;
	hash = 0x811c9dc5;

	while(*key_base)
		hash = (((dhash_key_t)(*(key_base++))) ^ hash) * 0x01000193;

	return hash;
}

_alwaysinline dhash_map_t *dhash_new(size_t value_size){
	dhash_map_t *ret;
	darray_t *darray;

	ret = malloc(sizeof(dhash_map_t));

	darray = darray_new(sizeof(dhash_pairing_t) + value_size);
	ret->darray = darray;

	ret->storage_unit = value_size;
	return ret;
}

_alwaysinline intptr_t _dhash_search_for_insert(dhash_map_t **hash_map, dhash_key_t key){
	intptr_t bottom, top, middle;

	if((*hash_map)->darray->length == 0)
		return 0;

	bottom = 0;
	top = (*hash_map)->darray->length - 1;

	while(top > bottom){
		middle = (bottom + top) / 2;

		if(key == ((dhash_pairing_t *)darray_get(&((*hash_map)->darray), middle))->key){
			return -1;
		}
		if(key > ((dhash_pairing_t *)darray_get(&((*hash_map)->darray), middle))->key){
			bottom = middle + 1;
		}
		else{
			top = middle - 1;
		}
	}

	return (key > ((dhash_pairing_t *)darray_get(&((*hash_map)->darray), bottom))->key) ? (bottom + 1) : bottom;
}

_alwaysinline intptr_t _dhash_search_for_get(dhash_map_t **hash_map, dhash_key_t key){
	intptr_t bottom, top, middle;

	if((*hash_map)->darray->length == 0)
		return -1;

	bottom = 0;
	top = (*hash_map)->darray->length - 1;

	while(top >= bottom){
		middle = (bottom + top) / 2;

		if(key == ((dhash_pairing_t *)darray_get(&((*hash_map)->darray), middle))->key){
			return middle;
		}
		else if(key > ((dhash_pairing_t *)darray_get(&((*hash_map)->darray), middle))->key){
			bottom = middle + 1;
		}
		else{
			top = middle - 1;
		}
	}

	return -1;
}

_alwaysinline int dhash_add(dhash_map_t **hash_map, const char *key_base, void *value){
	dhash_key_t key;
	uintptr_t index;
	int status;
	uint8_t pairing[(*hash_map)->darray->unit];
	darray_t *darray;

	key = _dhash_hash(key_base);

	dhash_pairing_t *pring = (dhash_pairing_t *)pairing;

	if(*hash_map == NULL)
		return 1;

	index =  _dhash_search_for_insert(hash_map, key);

	if(index == -1)
		return 1;

	pring->key = key;

	memcpy(pring->value, value, (*hash_map)->storage_unit);

	darray_insert(&((*hash_map)->darray), index, (void *)pring);

	return 0;
}

_alwaysinline int dhash_remove(dhash_map_t **hash_map, const char *key_base){
	dhash_key_t key;
	uintptr_t index;
	int status;
	darray_t *darray;

	key = _dhash_hash(key_base);

	if(*hash_map == NULL)
		return 1;

	index =  _dhash_search_for_insert(hash_map, key);

	if(index == -1)
		return 1;

	darray_remove(&((*hash_map)->darray), index);

	return 0;
}

_alwaysinline void *dhash_get(dhash_map_t **hash_map, const char *key_base){
	dhash_key_t key;
	uintptr_t index;
	int status;
	darray_t *darray;
	dhash_pairing_t *pairing;
	void *data;

	key = _dhash_hash(key_base);

	if(*hash_map == NULL)
		return NULL;

	index = _dhash_search_for_get(hash_map, key);

	if(index == -1)
		return NULL;

	pairing = darray_get(&((*hash_map)->darray), index);

	data = pairing->value;

	return data;
}

_alwaysinline int dhash_set(dhash_map_t **hash_map, const char *key_base, void *value){
	dhash_key_t key;
	uintptr_t index;
	int status;
	darray_t *darray;
	uint8_t buffer[(*hash_map)->darray->unit];
	dhash_pairing_t *pairing = (dhash_pairing_t *)buffer;

	key = _dhash_hash(key_base);

	if(*hash_map == NULL)
		return 1;

	index = _dhash_search_for_get(hash_map, key);

	if(index == -1)
		return 1;

	pairing->key = key;
	memcpy(pairing->value, value, (*hash_map)->storage_unit);

	status = darray_set(&((*hash_map)->darray), index, pairing);
	if(status == 1)
		return 1;

	return 0;
}

_alwaysinline int dhash_delete(dhash_map_t **hash_map){
	int status;

	if(*hash_map == NULL)
		return 1;

	status = darray_delete(&((*hash_map)->darray));
	if(status == 1)
		return 1;

	free(*hash_map);
	*hash_map = NULL;

	return 0;
}
#undef _alwaysinline

