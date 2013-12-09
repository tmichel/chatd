#ifndef _TYPES_H
#define _TYPES_H

#include "vec.h"
#include "string.h"

/********************** User types ************************/

typedef struct {
    char *username;
    int sock;
    vec_t *rooms;
} user_t;

/**********************************************************/

/********************** Room types ************************/

typedef struct __room {
    char *name;
    vec_t *admins;
    vec_t *users;
    vec_t *muted;
} room_t;

/**********************************************************/

/******************** Command types ***********************/

/* Command code, an alias for int */
typedef int cc_t;

struct __command_result {
    cc_t code;
    user_t *user;
};

struct __command {
    const cc_t code;
    string args;
};

typedef struct __command command_t;
typedef struct __command_result cr_t;

/**********************************************************/

#endif