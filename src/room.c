#include "const.h"
#include "room.h"
#include "command.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

static cr_t broadcast(room_t * const, char *);
static int is_admin(room_t * const, user_t * const);

room_t*
room_new(const char *name) {
    room_t* room = (room_t*)malloc(sizeof(room_t));

    // copy name
    int len = strlen(name);
    room->name = (char*)calloc(sizeof(char), sizeof(char) * (len + 1));
    strcpy(room->name, name);

    // init users, admin, banned
    room->users = vec_new();
    room->admins = vec_new();
    room->banned = vec_new();

    return room;
}

void
room_free(room_t *room) {
    free(room->name);
    vec_free(room->users);
    vec_free(room->admins);
    vec_free(room->banned);
    free(room);
}

cr_t
room_add_user(room_t *room, user_t * const user, int is_admin) {
    vec_add(room->users, user);
    if(is_admin) {
        vec_add(room->admins, user);
    }

    char buf[SYSMSG_BUF_SIZE] = {0};
    snprintf(buf, SYSMSG_BUF_SIZE, "* * * <%s> joined #%s.\n", user->username, room->name);
    return broadcast(room, buf);
}

cr_t
room_remove_user(room_t *room, user_t * const user) {
    // remove user from
    vec_remove(room->users, user);
    vec_remove(room->admins, user);
    vec_remove(room->banned, user);

    char buf[SYSMSG_BUF_SIZE] = {0};
    snprintf(buf, SYSMSG_BUF_SIZE, "* * * <%s> is leaving #%s\n", user->username, room->name);
    return broadcast(room, buf);
}

cr_t
room_send_msg(room_t * const room, user_t * const user, string msg) {
    // check if banned
    if (vec_contains(room->banned, user)) {
        return cr_create(CMD_RES_USR_BND);
    }

    // check if user is in the room
    if (!vec_contains(room->users, user)) {
        return cr_create(CMD_RES_NO_USR);
    }

    char *fmt = is_admin(room, user) ? MESSAGE_ADMIN_FORMAT : MESSAGE_FORMAT;
    char *message = (char*)calloc(sizeof(char), sizeof(char) * MAX_MESSAGE_SIZE );
    snprintf(message, MAX_MESSAGE_SIZE, fmt, room->name, user->username, msg.val);

    broadcast(room, message);

    free(message);
    return cr_ok();
}

static cr_t
broadcast(room_t * const room, char* message) {
    user_t *u;

    int err = 0;

    // broadcast message
    for (int i = 0; i < vec_size(room->users); ++i) {
        if (vec_get(room->users, i, (any_t*)&u) == 0) {
            if (write(u->sock, message, strlen(message)) < 0) {
                // TODO: log error
                ++err;
            }
        }
    }

    if (err == 0) {
        return cr_ok();
    }
    return cr_create(CMD_RES_WRT_ERR);
}

static int
is_admin(room_t* const room, user_t* const user) {
    return vec_contains(room->admins, user);
}
