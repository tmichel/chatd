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
 * Command results;
 * Values are >= 100
 */
#define CMM_RES_OK                100
#define CMD_RES_PARSE_ERR         101
#define CMD_RES_USER_INV_PASS     102

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

// User registration.
command_result_t user_reg(const command_t *cmd);

// Change user's password.
command_result_t user_change_pwd(user_t* const user, const command_t *cmd);

// User exit.
command_result_t user_exit(user_t* const user, const command_t *cmd);

#endif