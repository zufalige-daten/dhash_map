#include <darray.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>

int darray_grow(darray_t **array, size_t count){
	*array = (darray_t *)realloc(*array, sizeof(**array) + (((*array)->length + count) * (*array)->unit));

	if(*array == NULL)
		return 1;

	(*array)->length += count;
	return 0;
}

int darray_shrink(darray_t **array, size_t count){
	*array = (darray_t *)realloc(*array, sizeof(**array) + (((*array)->length - count) * (*array)->unit));

	if(*array == NULL)
		return 1;

	(*array)->length -= count;
	return 0;
}

darray_t *darray_new(size_t unit){
	darray_t *ret;

	ret = (darray_t *)malloc(sizeof(darray_t));

	if(ret == NULL)
		return ret;

	ret->length = 0;
	ret->unit = unit;
	return ret;
}

int darray_delete(darray_t **array){
	if(*array == NULL)
		return 1;

	free(*array);
	*array = 0;
	return 0;
}

int darray_add(darray_t **array, void *item){
	int status;

	if(*array == NULL)
		return 1;

	status = darray_grow(array, 1);
	if(status != 0)
		return 1;

	memcpy(&(*array)->array[((*array)->length - 1) * (*array)->unit], item, (*array)->unit);
	return 0;
}

int darray_insert(darray_t **array, uintptr_t index, void *item){
	size_t grow_size;
	int status;

	if(*array == NULL)
		return 1;

	if(index >= (*array)->length){
		grow_size = index - ((*array)->length - 1);
	}
	else{
		grow_size = 1;
	}
	status = darray_grow(array, grow_size);
	if(status != 0)
		return 1;

	memcpy(&(*array)->array[(index + 1) * (*array)->unit], &(*array)->array[index * (*array)->unit], ((*array)->length - 1 - index) * (*array)->unit);

	memcpy(&(*array)->array[index * (*array)->unit], item, (*array)->unit);

	return 0;
}

int darray_remove(darray_t **array, uintptr_t index){
	int status;

	if(*array == NULL)
		return 1;

	if(index >= (*array)->length)
		return 1;

	memcpy(&(*array)->array[index * (*array)->unit], &(*array)->array[(index + 1) * (*array)->unit], ((*array)->length - 1 - index) * (*array)->unit);

	status = darray_shrink(array, 1);
	if(status != 0)
		return 1;

	return 0;
}

int darray_set(darray_t **array, uintptr_t index, void *item){
	if(*array == NULL)
		return 1;

	if(index >= (*array)->length)
		return 1;

	memcpy(&(*array)->array[index * (*array)->unit], item, (*array)->unit);
	return 0;
}

void * darray_get(darray_t **array, uintptr_t index){
	if(*array == NULL)
		return NULL;

	if(index >= (*array)->length)
		return NULL;

	return &(*array)->array[index * (*array)->unit];
}

