#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "vec.h"

vec_t *users = NULL;
vec_t *rooms = NULL;

static int user_eq(any_t, any_t);
static int room_eq(any_t, any_t);

void
mem_init() {
    users = vec_new_cap(100);
    rooms = vec_new_cap(100);
}

mem_res
mem_store_user(user_t *user) {
    vec_add(users, user);
    return MEM_OK;
}

mem_res
mem_remove_user(user_t* const user) {
    int res = vec_remove(users, user);
    return (res ? MEM_OK : MEM_NOTFOUND);
}

mem_res
mem_lookup_user(const char *username, user_t **user) {
    user_t *u = vec_find(users, (any_t)username, user_eq);

    if (u == NULL) {
        return MEM_NOTFOUND;
    }

    *user = u;
    return MEM_OK;
}

mem_res
mem_store_room(room_t *room) {
    vec_add(rooms, room);
    return MEM_OK;
}

mem_res
mem_remove_room(room_t* const room) {
    int res = vec_remove(rooms, room);
    return (res ? MEM_OK : MEM_NOTFOUND);
}


mem_res
mem_lookup_room(const char *room_name, room_t **room) {
    room_t *r = vec_find(rooms, (any_t)room_name, room_eq);

    if (r == NULL) {
        return MEM_NOTFOUND;
    }

    *room = r;
    return MEM_OK;
}

static int
user_eq(any_t name, any_t user) {
    char *username = (char*)name;
    user_t *u = (user_t*)user;

    return strcmp(username, u->username) == 0;
}

static int
room_eq(any_t name, any_t room) {
    char *rname = (char*)name;
    room_t *r = (room_t*)room;

    return strcmp(rname, r->name) == 0;
}
