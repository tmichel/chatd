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

void vec_free(vec_t *vec);

void vec_add(vec_t *vec, any_t value);

void vec_remove(vec_t *vec, any_t value);

int vec_get(vec_t *vec, int idx, any_t *res);

any_t vec_set(vec_t *vec, int idx, any_t val);

int vec_size(vec_t *vec);

#endif
