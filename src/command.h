#ifndef _COMMNAD_H
#define _COMMNAD_H

#include "types.h"

/* New command with code and args */
command_t command_new(cc_t, string);

// Frees the command struct and its content
void command_destroy(command_t cmd);

cr_t cr_create(cc_t code);
cr_t cr_empty();
cr_t cr_ok();
cr_t cr_parse_err();

/**
 * Execute command.
 */
cr_t command_execute(command_t cmd, user_t * const user);

#endif
