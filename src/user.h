#ifndef _USER_H
#define _USER_H

#include "vec.h"

typedef struct {
    char *token;
    char *username;
    int sock;
} user_t;

user_t* new_user();

user_t* new_user_with_name(const char *username);

void free_user(user_t *user);

#endif
