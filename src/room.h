#ifndef _ROOM_H
#define _ROOM_H

#include "user.h"

typedef struct {
    char *name;
    vec_t *admins;
    vec_t *users;
} room_t;

room_t *room_new(const char *name);

void room_free(room_t *room);

void room_add_user(room_t *room, user_t *user, int is_admin);

#endif
