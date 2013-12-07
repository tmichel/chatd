#ifndef _VEC_H
#define _VEC_H

typedef void* any_t;

typedef struct
{
    int len;
    int cap;
    any_t *data;
} vec_t;

vec_t *vec_new();

vec_t *vec_new_cap(int);

void vec_free(vec_t *vec);

void vec_add(vec_t *vec, any_t value);

int vec_remove(vec_t *vec, any_t value);

int vec_get(vec_t *vec, int idx, any_t *res);

any_t vec_set(vec_t *vec, int idx, any_t val);

int vec_size(vec_t *vec);

int vec_is_empty(vec_t*);

int vec_contains(vec_t*, any_t);

/**
 * Find an item in the vector.
 *
 * The third parameter is a test function which should return true (1) when
 * the item is found. The second parameter will be passed to the test function
 * as the first parameter.
 *
 * If there was no item that returned true for the test function NULL will be
 * returned.
 */
any_t vec_find(vec_t*, any_t, int (*)(any_t, any_t));

#endif
