#ifndef _COMMNAD_H
#define _COMMNAD_H

#include "user.h"
#include "string.h"

/* Command code, an alias for int */
typedef int cc_t;

struct __command_result {
    cc_t code;
    char *msg;
    user_t *user;
};

struct __command {
    const cc_t code;
    string args;
};

typedef struct __command command_t;
typedef struct __command_result cr_t;

/* New command with code and args */
command_t command_new(cc_t, string);

// Frees the command struct and its content
void command_destroy(command_t cmd);

cr_t cr_create(cc_t code, char *msg);

cr_t cr_ok();

cr_t cr_init();

/**
 * Execute command.
 */
cr_t command_execute(command_t cmd, user_t * const user);

#endif
