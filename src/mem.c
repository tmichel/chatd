#include <stdlib.h>
#include <string.h>
#include "mem.h"

#define MAX_USER_COUNT 1000
#define MAX_ROOM_COUNT 1000

int last_usr;
int last_room;
// TODO: use vectors here
user_t *users[MAX_USER_COUNT];
room_t *rooms[MAX_ROOM_COUNT];

mem_res
mem_init() {
    memset(users, 0, sizeof(user_t*) * MAX_USER_COUNT);
    memset(rooms, 0, sizeof(room_t*) * MAX_ROOM_COUNT);
    last_usr = 0;
    last_room = 0;
    return MEM_OK;
}

mem_res
mem_store_user(user_t *user) {
    if (last_usr >= MAX_USER_COUNT) {
        return MEM_FULL;
    }

    users[last_usr] = user;
    ++last_usr;

    return MEM_OK;
}

mem_res
mem_lookup_user(const char *username, user_t **user) {
    user_t *user_l;
    for (int i = 0; i < last_usr; ++i) {
        user_l = users[i];

        if (strcmp(user_l->username, username) == 0) {
            *user = user_l;
            return MEM_OK;
        }
    }

    return MEM_NOTFOUND;
}

mem_res
mem_store_room(room_t *room) {
    return MEM_OK;
}

mem_res
mem_lookup_room(const char *room_name, room_t **room) {
    return MEM_NOTFOUND;
}