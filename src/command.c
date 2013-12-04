#include <stdlib.h>

#include "command.h"

command_t* new_command() {
    command_t *cmd = (command_t*)malloc(sizeof(command_t));
    cmd->code = CMD_PARSE_ERROR;
    cmd->args = NULL;

    return cmd;
}

void free_command(command_t* cmd) {
    free(cmd->args);
    free(cmd);
}
