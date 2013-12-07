#include "vec.h"

#include <stdio.h>
#include <stdlib.h>

vec_t *vec_new() {
    vec_t *vec = (vec_t*)malloc(sizeof(vec_t*));
    vec->len = 0;
    vec->cap = 10;
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
    ++vec->len;
}

void vec_remove(vec_t *vec, any_t value) {
    printf("vec_removed called. implement it first");
    exit(1);
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

int vec_contains(vec_t* vec, any_t val) {
    for (int i = 0; i < vec->len; ++i) {
        if (val == vec->data[i]) {
            return 1;
        }
    }

    return 0;
}

