#ifndef _USER_H
#define _USER_H

#include "vec.h"

typedef struct {
    char *token;
    char *username;
    int sock;
} user_t;

typedef struct {
    char *name;
    vec_t *admins;
    vec_t *users;
} room_t;

user_t* new_user();

user_t* new_user_with_name(const char *username);

void free_user(user_t *user);

room_t *new_room(const char *name);

void free_room(room_t *room);

#endif