#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>

typedef struct{
	size_t length;
	size_t unit;
	uint8_t array[];
} darray_t;

extern int darray_grow(darray_t **array, size_t count);
extern int darray_shrink(darray_t **array, size_t count);
extern darray_t *darray_new(size_t unit);
extern int darray_delete(darray_t **array);
extern int darray_add(darray_t **array, void *item);
extern int darray_insert(darray_t **array, uintptr_t index, void *item);
extern int darray_remove(darray_t **array, uintptr_t index);
extern int darray_set(darray_t **array, uintptr_t index, void *item);
extern void * darray_get(darray_t **array, uintptr_t index);

