#include "vec.h"

#include <stdio.h>
#include <stdlib.h>

vec_t *vec_new() {
    return vec_new_cap(10);
}

vec_t *vec_new_cap(int cap) {
    vec_t *vec = (vec_t*)malloc(sizeof(vec_t*));
    vec->len = 0;
    vec->cap = cap;
    vec->data = (any_t*)calloc(sizeof(any_t), sizeof(any_t) * vec->cap);

    return vec;
}

void vec_free(vec_t *vec) {
    free(vec->data);
    free(vec);
}

void vec_add(vec_t *vec, any_t value) {
    // make the underlying array bigger
    if (vec->len >= vec-> cap) {
        vec->cap *= 2;
        vec->data = realloc(vec->data, sizeof(any_t) * vec->cap);
    }

    vec->data[vec->len] = value;
    ++(vec->len);
}

int vec_remove(vec_t *vec, any_t value) {
    int i = 0;
    // get the index
    while (i < vec->len && vec->data[i] != value) ;

    if (i >= vec->len)
        // could not find it
        return 0;

    // rearrange data
    for (int j = i + 1; j < vec->len; ++j) {
        vec->data[j-1] = vec->data[j];
    }

    --(vec->len);

    return 1;
}

int vec_get(vec_t *vec, int idx, any_t* res) {
    if (idx >= vec->len) {
        return -1;
    }

    *res = vec->data[idx];
    return 0;
}

any_t vec_set(vec_t *vec, int idx, any_t val) {
    if (idx >= vec->len) {
        return NULL;
    }

    any_t old = vec->data[idx];
    vec->data[idx] = val;

    return old;
}

int vec_size(vec_t *vec) {
    return vec->len;
}

int vec_is_empty(vec_t *vec) {
    return vec->len == 0;
}

int vec_contains(vec_t* vec, any_t val) {
    for (int i = 0; i < vec->len; ++i) {
        if (val == vec->data[i]) {
            return 1;
        }
    }

    return 0;
}

