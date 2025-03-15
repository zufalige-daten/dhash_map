#pragma once

#include <stdint.h>
#include <darray.h>
#include <stdlib.h>

typedef uint64_t dhash_key_t;

#define DHASH_MAP_TUPLE(prefix, T) typedef struct prefix##_##T##_dhash_pairing { \
	dhash_key_t key; \
	T value; \
} prefix##_##T##_dhash_pairing_t;

#define DHASH_MAP_WITH(prefix, T) typedef struct prefix##_##T##_dhash_map { \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_t *darray; \
} prefix##_##T##_dhash_map_t; \
 \
inline __attribute__((always_inline)) dhash_key_t _##prefix##_##T##_dhash_hash(const char *key_base){ \
	dhash_key_t hash; \
	hash = 0x811c9dc5; \
 \
	while(*key_base) \
		hash = (((dhash_key_t)(*(key_base++))) ^ hash) * 0x01000193; \
 \
	return hash; \
} \
 \
inline __attribute__((always_inline)) prefix##_##T##_dhash_map_t *prefix##_##T##_dhash_new(){ \
	prefix##_##T##_dhash_map_t *ret; \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_t *darray; \
 \
	ret = malloc(sizeof(prefix##_##T##_dhash_map_t)); \
 \
	darray = prefix##_##prefix##_##T##_dhash_pairing_t_darray_new(); \
	ret->darray = darray; \
 \
	return ret; \
} \
 \
inline __attribute__((always_inline)) intptr_t _##prefix##_##T##_dhash_search_for_insert(prefix##_##T##_dhash_map_t **hash_map, dhash_key_t key){ \
	intptr_t bottom, top, middle; \
 \
	if((*hash_map)->darray->length == 0) \
		return 0; \
 \
	bottom = 0; \
	top = (*hash_map)->darray->length - 1; \
 \
	while(top > bottom){ \
		middle = (bottom + top) / 2; \
 \
		if(key == ((prefix##_##T##_dhash_pairing_t *)prefix##_##prefix##_##T##_dhash_pairing_t_darray_get(&((*hash_map)->darray), middle))->key){ \
			return -1; \
		} \
		if(key > ((prefix##_##T##_dhash_pairing_t *)prefix##_##prefix##_##T##_dhash_pairing_t_darray_get(&((*hash_map)->darray), middle))->key){ \
			bottom = middle + 1; \
		} \
		else{ \
			top = middle - 1; \
		} \
	} \
 \
	return (key > ((prefix##_##T##_dhash_pairing_t *)prefix##_##prefix##_##T##_dhash_pairing_t_darray_get(&((*hash_map)->darray), bottom))->key) ? (bottom + 1) : bottom; \
} \
 \
inline __attribute__((always_inline)) intptr_t _##prefix##_##T##_dhash_search_for_get(prefix##_##T##_dhash_map_t **hash_map, dhash_key_t key){ \
	intptr_t bottom, top, middle; \
 \
	if((*hash_map)->darray->length == 0) \
		return -1; \
 \
	bottom = 0; \
	top = (*hash_map)->darray->length - 1; \
 \
	while(top >= bottom){ \
		middle = (bottom + top) / 2; \
 \
		if(key == ((prefix##_##T##_dhash_pairing_t *)prefix##_##prefix##_##T##_dhash_pairing_t_darray_get(&((*hash_map)->darray), middle))->key){ \
			return middle; \
		} \
		else if(key > ((prefix##_##T##_dhash_pairing_t *)prefix##_##prefix##_##T##_dhash_pairing_t_darray_get(&((*hash_map)->darray), middle))->key){ \
			bottom = middle + 1; \
		} \
		else{ \
			top = middle - 1; \
		} \
	} \
 \
	return -1; \
} \
 \
inline __attribute__((always_inline)) int prefix##_##T##_dhash_add(prefix##_##T##_dhash_map_t **hash_map, const char *key_base, T value){ \
	dhash_key_t key; \
	uintptr_t index; \
	int status; \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_t *darray; \
 \
	key = _##prefix##_##T##_dhash_hash(key_base); \
 \
	if(*hash_map == NULL) \
		return 1; \
 \
	index =  _##prefix##_##T##_dhash_search_for_insert(hash_map, key); \
 \
	if(index == -1) \
		return 1; \
 \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_insert(&((*hash_map)->darray), index, (prefix##_##T##_dhash_pairing_t) {.key=key, .value=value}); \
 \
	return 0; \
} \
 \
inline __attribute__((always_inline)) int prefix##_##T##_dhash_remove(prefix##_##T##_dhash_map_t **hash_map, const char *key_base){ \
	dhash_key_t key; \
	uintptr_t index; \
	int status; \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_t *darray; \
 \
	key = _##prefix##_##T##_dhash_hash(key_base); \
 \
	if(*hash_map == NULL) \
		return 1; \
 \
	index =  _##prefix##_##T##_dhash_search_for_insert(hash_map, key); \
 \
	if(index == -1) \
		return 1; \
 \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_remove(&((*hash_map)->darray), index); \
 \
	return 0; \
} \
 \
inline __attribute__((always_inline)) T *prefix##_##T##_dhash_get(prefix##_##T##_dhash_map_t **hash_map, const char *key_base){ \
	dhash_key_t key; \
	uintptr_t index; \
	int status; \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_t *darray; \
	prefix##_##T##_dhash_pairing_t *pairing; \
	T *data; \
 \
	key = _##prefix##_##T##_dhash_hash(key_base); \
 \
	if(*hash_map == NULL) \
		return NULL; \
 \
	index = _##prefix##_##T##_dhash_search_for_get(hash_map, key); \
 \
	if(index == -1) \
		return NULL; \
 \
	pairing = prefix##_##prefix##_##T##_dhash_pairing_t_darray_get(&((*hash_map)->darray), index); \
 \
	data = &pairing->value; \
 \
	return data; \
} \
 \
inline __attribute__((always_inline)) int prefix##_##T##_dhash_set(prefix##_##T##_dhash_map_t **hash_map, const char *key_base, T value){ \
	dhash_key_t key; \
	uintptr_t index; \
	int status; \
	prefix##_##prefix##_##T##_dhash_pairing_t_darray_t *darray; \
 \
	key = _##prefix##_##T##_dhash_hash(key_base); \
 \
	if(*hash_map == NULL) \
		return 1; \
 \
	index = _##prefix##_##T##_dhash_search_for_get(hash_map, key); \
 \
	if(index == -1) \
		return 1; \
 \
	status = prefix##_##prefix##_##T##_dhash_pairing_t_darray_set(&((*hash_map)->darray), index, (prefix##_##T##_dhash_pairing_t) {.key=key, .value=value}); \
	if(status == 1) \
		return 1; \
 \
	return 0; \
} \
 \
inline __attribute__((always_inline)) int prefix##_##T##_dhash_delete(prefix##_##T##_dhash_map_t **hash_map){ \
	int status; \
 \
	if(*hash_map == NULL) \
		return 1; \
 \
	status = prefix##_##prefix##_##T##_dhash_pairing_t_darray_delete(&((*hash_map)->darray)); \
	if(status == 1) \
		return 1; \
 \
	free(*hash_map); \
	*hash_map = NULL; \
 \
	return 0; \
} \
 \
inline __attribute__((always_inline)) prefix##_##T##_dhash_pairing_t *prefix##_##T##_dhash_retrive(prefix##_##T##_dhash_map_t **hash_map, uintptr_t index){ \
	prefix##_##T##_dhash_pairing_t *ret; \
 \
	ret = prefix##_##prefix##_##T##_dhash_pairing_t_darray_get(&((*hash_map)->darray), index); \
 \
	return ret; \
}

#define DHASH_MAP_USE(prefix, T) DHASH_MAP_TUPLE(prefix, T); \
DARRAY_USE(prefix, prefix##_##T##_dhash_pairing_t); \
DHASH_MAP_WITH(prefix, T);

