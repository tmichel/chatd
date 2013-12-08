#include "vec.h"

#include <stdlib.h>
#include <pthread.h>

vec_t *vec_new() {
    return vec_new_cap(10);
}

vec_t *vec_new_cap(int cap) {
    vec_t *vec = (vec_t*)malloc(sizeof(vec_t*));
    vec->len = 0;
    vec->cap = cap;
    vec->data = (any_t*)calloc(sizeof(any_t), sizeof(any_t) * vec->cap);
    vec->lock = (pthread_rwlock_t*)malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(vec->lock, NULL);

    return vec;
}

void vec_free(vec_t *vec) {
    pthread_rwlock_destroy(vec->lock);
    free(vec->lock);
    free(vec->data);
    free(vec);
}

void vec_add(vec_t *vec, any_t value) {
    // acquire write lock
    pthread_rwlock_wrlock(vec->lock);

    // make the underlying array bigger
    if (vec->len >= vec-> cap) {
        vec->cap *= 2;
        vec->data = realloc(vec->data, sizeof(any_t) * vec->cap);
    }

    vec->data[vec->len] = value;
    ++(vec->len);

    // release lock
    pthread_rwlock_unlock(vec->lock);
}

int vec_remove(vec_t *vec, any_t value) {
    int i = 0;

    // searching for the item only needs a read lock
    pthread_rwlock_rdlock(vec->lock);
    // get the index
    while (i < vec->len && vec->data[i] != value) ;
    pthread_rwlock_unlock(vec->lock);

    if (i >= vec->len) {
        // could not find it
        return 0;
    }

    pthread_rwlock_wrlock(vec->lock);

    // NULL out
    vec->data[i] = NULL;

    // rearrange data
    for (int j = i + 1; j < vec->len; ++j) {
        vec->data[j-1] = vec->data[j];
    }

    --(vec->len);
    pthread_rwlock_unlock(vec->lock);

    return 1;
}

int vec_get(vec_t *vec, int idx, any_t* res) {
    if (idx >= vec->len) {
        return -1;
    }

    pthread_rwlock_rdlock(vec->lock);
    *res = vec->data[idx];
    pthread_rwlock_unlock(vec->lock);
    return 0;
}

any_t vec_set(vec_t *vec, int idx, any_t val) {
    if (idx >= vec->len) {
        return NULL;
    }

    pthread_rwlock_wrlock(vec->lock);
    any_t old = vec->data[idx];
    vec->data[idx] = val;
    pthread_rwlock_unlock(vec->lock);

    return old;
}

int vec_size(vec_t *vec) {
    return vec->len;
}

int vec_is_empty(vec_t *vec) {
    return vec->len == 0;
}

int vec_contains(vec_t* vec, any_t val) {
    int res = 0;
    pthread_rwlock_rdlock(vec->lock);

    for (int i = 0; i < vec->len; ++i) {
        if (val == vec->data[i]) {
            res = 1;
            break;
        }
    }

    pthread_rwlock_unlock(vec->lock);
    return res;
}

any_t vec_find(vec_t *vec, any_t val, int (*predicate)(any_t, any_t)) {
    any_t res = NULL;

    pthread_rwlock_rdlock(vec->lock);
    for (int i = 0; i < vec->len; ++i) {
        if (predicate(val, vec->data[i])) {
            res = vec->data[i];
        }
    }
    pthread_rwlock_unlock(vec->lock);

    return res;
}

