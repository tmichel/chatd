#include "room.h"

#include <stdlib.h>
#include <string.h>

room_t*
room_new(const char *name) {
    room_t* room = (room_t*)malloc(sizeof(room_t));

    // copy name
    int len = strlen(name);
    room->name = (char*)calloc(sizeof(char), sizeof(char) * (len + 1));
    strcpy(room->name, name);

    // init users, admin
    room->users = vec_new();
    room->admins = vec_new();

    return room;
}

void
room_free(room_t *room) {
    free(room->name);
    vec_free(room->users);
    vec_free(room->admins);
    free(room);
}

void
room_add_user(room_t *room, user_t *user, int is_admin) {
    vec_add(room->users, user);
    if(is_admin) {
        vec_add(room->admins, user);
    }
}
