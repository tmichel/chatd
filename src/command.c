#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "command.h"
#include "mem.h"

static void make_result(command_result_t *res, command_code_t code, char *msg);
static void command_ok(command_result_t *res, char *msg);
static void command_err(command_result_t *res, char *msg);
static void command_parse_error(command_result_t *res);

command_t*
new_command() {
    command_t *cmd = (command_t*)malloc(sizeof(command_t));
    cmd->code = CMD_PARSE_ERROR;
    cmd->args = NULL;

    return cmd;
}

void
free_command(command_t* cmd) {
    free(cmd->args);
    free(cmd);
}


command_result_t
command_execute(user_t *user, command_t *cmd) {
    command_result_t res;

    switch (cmd->code) {
    case CMD_REG:
        return user_reg(cmd);
    default:
        command_parse_error(&res);
    }

    return res;
}

command_result_t
user_reg(const command_t *cmd) {
    char *saveptr;
    char *username;
    char *password;
    command_result_t res;

    username = strtok_r(cmd->args, COMMAND_DELIM, &saveptr);

    // if we could not find a username
    if (username == NULL) {
        command_parse_error(&res);
        return res;
    }
    password = strtok_r(NULL, COMMAND_DELIM, &saveptr);

    user_t *user = NULL;
    mem_res lookup_res = mem_lookup_user(username, &user);

    if (lookup_res == MEM_OK) {
        command_err(&res, "ERROR: Cannot login twice.");
        return res;
    }

    if (lookup_res == MEM_NOTFOUND) {
        // password was specified. login or register
        if (password != NULL) {
            // TODO: lookup from persistent store and authenticate
            // user = ...

            // atm we dont have a persistent store, so create a new user
            user = new_user_with_name(username);
        } else {
            user = new_user_with_name(username);
        }
        mem_store_user(user);
    }

    char msg[100];
    sprintf(msg, "Welcome %s. Token: %s", user->username, user->token);

    command_ok(&res, msg);
    return res;
}

static void
make_result(command_result_t *res, command_code_t code, char *msg) {
    res->code = code;

    int len = strlen(msg);
    res->msg = (char*)calloc(sizeof(char), sizeof(char) * len);
    strcpy(res->msg, msg);
}

static void
command_ok(command_result_t *res, char *msg) {
    make_result(res, CMD_RES_OK, msg);
}

static void
command_err(command_result_t *res, char *msg) {
    make_result(res, CMD_RES_ERR, msg);
}

static void
command_parse_error(command_result_t *res) {
    command_err(res, "ERROR: could not parse command.");
}
