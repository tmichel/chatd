#include "const.h"
#include "room.h"
#include "command.h"
#include "user.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

static cr_t broadcast(room_t * const, char *);
static int is_admin(room_t * const, user_t * const);
static int admin_prep(room_t* const, user_t* const, string, cr_t*, user_t**);
static cr_t remove_user(room_t* const, user_t* const, char*);

room_t*
room_new(const char *name) {
    room_t* room = (room_t*)malloc(sizeof(room_t));

    // copy name
    int len = strlen(name);
    room->name = (char*)calloc(sizeof(char), sizeof(char) * (len + 1));
    strcpy(room->name, name);

    // init users, admin, muted
    room->users = vec_new();
    room->admins = vec_new();
    room->muted = vec_new();

    return room;
}

void
room_free(room_t *room) {
    free(room->name);
    vec_free(room->users);
    vec_free(room->admins);
    vec_free(room->muted);
    free(room);
}

cr_t
room_add_user(room_t *room, user_t * const user, int admin) {
    vec_add(room->users, user);
    if(admin) {
        vec_add(room->admins, user);
    }

    // add room to user's rooms
    vec_add(user->rooms, room);

    char buf[SYSMSG_BUF_SIZE] = {0};
    snprintf(buf, SYSMSG_BUF_SIZE, "#%s * * * <%s> joined.\n", room->name, user->username);
    return broadcast(room, buf);
}

cr_t
room_remove_user(room_t *room, user_t * const user) {
    // broadcast user leaving
    char buf[SYSMSG_BUF_SIZE] = {0};
    snprintf(buf, SYSMSG_BUF_SIZE, "#%s * * * <%s> is leaving...\n", room->name, user->username);

    return remove_user(room, user, buf);
}

cr_t
room_send_msg(room_t * const room, user_t * const user, string msg) {
    // check if muted
    if (vec_contains(room->muted, user)) {
        return cr_create(CMD_RES_USR_MUTE);
    }

    // check if user is in the room
    if (!vec_contains(room->users, user)) {
        return cr_create(CMD_RES_NO_USR);
    }

    char *fmt = is_admin(room, user) ? MESSAGE_ADMIN_FORMAT : MESSAGE_FORMAT;
    char *message = (char*)calloc(sizeof(char), sizeof(char) * MAX_MESSAGE_SIZE );
    snprintf(message, MAX_MESSAGE_SIZE, fmt, room->name, user->username, msg.val);

    cr_t res = broadcast(room, message);

    free(message);
    return res;
}

cr_t
room_admin(room_t* const room, user_t* const admin, string username) {
    user_t *user = NULL;
    cr_t res = cr_empty();
    if (!admin_prep(room, admin, username, &res, &user)) {
        return res;
    }

    // grant admin rights
    vec_add(room->admins, user);

    char buf[MAX_MESSAGE_SIZE] = {0};
    snprintf(buf, MAX_MESSAGE_SIZE, "#%s * * * %s was granted admin rights.\n", room->name, user->username);
    return broadcast(room, buf);
}

cr_t
room_mute_user(room_t* const room, user_t* const admin, string username) {
    user_t *user = NULL;
    cr_t res = cr_empty();
    if (!admin_prep(room, admin, username, &res, &user)) {
        return res;
    }

    vec_add(room->muted, user);

    char buf[MAX_MESSAGE_SIZE] = {0};
    snprintf(buf, MAX_MESSAGE_SIZE, "#%s * * * %s was muted by %s.\n", room->name, user->username, admin->username);
    return broadcast(room, buf);
}

cr_t
room_voice_user(room_t* const room, user_t* const admin, string username) {
    user_t *user = NULL;
    cr_t res = cr_empty();
    if (!admin_prep(room, admin, username, &res, &user)) {
        return res;
    }

    vec_remove(room->muted, user);

    char buf[MAX_MESSAGE_SIZE] = {0};
    snprintf(buf, MAX_MESSAGE_SIZE, "#%s * * * %s can speak again.\n", room->name, user->username);
    return broadcast(room, buf);
}

cr_t
room_kick_user(room_t* const room, user_t* const admin, string username) {
    user_t *user = NULL;
    cr_t res = cr_empty();
    if (!admin_prep(room, admin, username, &res, &user)) {
        return res;
    }

    char buf[SYSMSG_BUF_SIZE];
    snprintf(buf, SYSMSG_BUF_SIZE, "#%s * * * <%s> was kicked by %s.\n", room->name, user->username, admin->username);
    return remove_user(room, user, buf);
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

static int
admin_prep(room_t* const room, user_t* const admin, string username, cr_t *res, user_t **user) {
    // check if user has rights to do this
    if (!is_admin(room, admin)) {
        res->code = CMD_RES_NOT_ALLOWED;
        return 0;
    }

    // get user from room's users
    user_t *u = vec_find(room->users, (any_t)username.val, user_eq);
    if (u == NULL) {
        res->code = CMD_RES_NO_USR;
        return 0;
    }

    *user = u;

    return 1;
}

static cr_t
remove_user(room_t* const room, user_t* const user, char* msg) {
    cr_t res = broadcast(room, msg);

    // remove user from
    vec_remove(room->users, user);
    vec_remove(room->admins, user);
    vec_remove(room->muted, user);
    // remove room from user's list
    vec_remove(user->rooms, room);

    return res;
}
