#include <stdlib.h>

#include "command.h"

static void command_ok(command_result_t *res, char *msg);
static void command_err(command_result_t *res);

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

    // handle parse error. immediate return, no further steps
    if (cmd->code == CMD_PARSE_ERROR) {
        command_err(&res);
        return res;
    }

    command_ok(&res, "Exiting");

    return res;
}

static void
command_ok(command_result_t * res, char *msg) {
    res->code = CMD_RES_OK;
    res->msg = msg;
}

static void
command_err(command_result_t *res) {
    res->code = CMD_RES_PARSE_ERR;
    res->msg = "ERROR: could not parse command.";
}
