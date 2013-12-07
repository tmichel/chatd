#ifndef _COMMNAD_H
#define _COMMNAD_H

#include "user.h"

/*
 * Command codes
 * Detailed information can be found in the PROTOCOL.md file (in Hungarian).
 */

#define CMD_PARSE_ERROR -1

/*
 * User signup and authentication.
 *
 * REG username [password]
 */
#define CMD_REG 1
#define CMD_REG_STR "REG"

/*
 * Client exits.
 *
 * EXIT
 */
#define CMD_EXIT 2
#define CMD_EXIT_STR "EXIT"

/*
 * Change password, or create one.
 *
 * PWD new [old]
 */
#define CMD_PWD 3
#define CMD_PWD_STR "PWD"

/*
 * User joins room.
 *
 * JOIN room
 */
#define CMD_JOIN 4
#define CMD_JOIN_STR "JOIN"

/*
 * User talks in a room.
 *
 * TALK room message
 */
#define CMD_TALK 5
#define CMD_TALK_STR "TALK"

/*
 * Command results;
 * Values are >= 100
 */
#define CMD_RES_OK      100
#define CMD_RES_ERR     101
#define CMD_RES_INV_TOK 102
#define CMD_RES_NO_USR  103

typedef int command_code_t;

typedef struct {
    command_code_t code;
    char *msg;
} command_result_t;

typedef struct
{
    command_code_t code;
    char *args;
} command_t;

// Instantiates a new command struct.
command_t* new_command();

// Frees the command struct and its content
void free_command(command_t* cmd);

/**
 * Execute command.
 */
command_result_t command_execute(user_t * const user, command_t *cmd);

#endif
