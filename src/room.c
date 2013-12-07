#include "room.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_MESSAGE_SIZE 2048
#define MESSAGE_FORMAT "[%s] <%s> %s"

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
room_add_user(room_t *room, user_t * const user, int is_admin) {
    vec_add(room->users, user);
    if(is_admin) {
        vec_add(room->admins, user);
    }
}

cr_t
room_send_msg(room_t * const room, user_t * const user, string msg) {
    // check if banned
    if (vec_contains(room->banned, user)) {
        return cr_create(CMD_RES_USR_BND, "ERROR: you are not allowed to talk");
    }

    // check if user is in the room
    if (!vec_contains(room->users, user)) {
        return cr_create(CMD_RES_NO_USR,
            "ERROR: cannot send a message into a room that you are not in.");
    }

    user_t *u;

    char *message = (char*)calloc(sizeof(char), sizeof(char) * MAX_MESSAGE_SIZE );
    snprintf(message, MAX_MESSAGE_SIZE, MESSAGE_FORMAT, room->name, user->username, msg.val);

    // broadcast message
    for (int i = 0; i < vec_size(room->users); ++i) {
        if (vec_get(room->users, i, (any_t*)&u) == 0) {
            write(u->sock, message, strlen(message));
        }
    }

    free(message);
    return cr_ok();
}
