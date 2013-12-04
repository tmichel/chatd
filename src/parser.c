#include <string.h>
#include <stdlib.h>

#include "parser.h"

static command_code_t
parse_command(char *command) {
    if (strcmp(command, CMD_REG_STR) == 0) {
        return CMD_REG;
    }
    if (strcmp(command, CMD_EXIT_STR) == 0) {
        return CMD_EXIT;
    }
    if (strcmp(command, CMD_PWD_STR) == 0) {
        return CMD_PWD;
    }

    return CMD_PARSE_ERROR;
}

void parse(const char *input, command_t *cmd) {
    size_t input_len = strlen(input);
    char input_t[input_len];

    strcpy(input_t, input);
    char* command_str = strtok(input_t, COMMAND_DELIM);

    command_code_t code = parse_command(command_str);
    size_t cmd_len = strlen(command_str);

    cmd->code = code;

    if (code == CMD_PARSE_ERROR) {
        // on parse error do not parse the arguments
        return;
    }

    // only when there is a leftover
    if (cmd_len < input_len) {
        // do not copy the extra space after the command
        cmd->args = (char*)calloc(sizeof(char), input_len - cmd_len - 1);
        strcpy(cmd->args, command_str + cmd_len + 1);
    }
}
