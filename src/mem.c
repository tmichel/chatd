#include <stdlib.h>
#include <string.h>
#include "mem.h"

#define MAX_USER_COUNT 1000

int last;
user_t *users[MAX_USER_COUNT];

mem_res
mem_init() {
    memset(users, 0, sizeof(user_t*) * MAX_USER_COUNT);
    last = 0;
    return MEM_OK;
}

mem_res
mem_store_user(user_t *user) {
    if (last >= MAX_USER_COUNT) {
        return MEM_FULL;
    }

    users[last] = user;
    ++last;

    return MEM_OK;
}

mem_res
mem_lookup_user(const char *username, user_t **user) {
    user_t *user_l;
    for (int i = 0; i < last; ++i) {
        user_l = users[i];

        if (strcmp(user_l->username, username) == 0) {
            *user = user_l;
            return MEM_OK;
        }
    }

    return MEM_NOTFOUND;
}