#ifndef _COMMNAD_H
#define _COMMNAD_H

#include "user.h"
#include "string.h"

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

struct __command_result {
    command_code_t code;
    char *msg;
};

struct __command {
    const command_code_t code;
    string args;
};

typedef struct __command command_t;
typedef struct __command_result command_result_t;

/* New command with code and args */
command_t command_new(command_code_t, string);

// Frees the command struct and its content
void command_free(command_t cmd);

/**
 * Execute command.
 */
command_result_t command_execute(command_t cmd, user_t * const user);

#endif
