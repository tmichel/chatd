#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "util.h"

static command_code_t
parse_command(string cmd_in) {
    if (strcmp(cmd_in.val, CMD_REG_STR) == 0) {
        return CMD_REG;
    }
    if (strcmp(cmd_in.val, CMD_EXIT_STR) == 0) {
        return CMD_EXIT;
    }
    if (strcmp(cmd_in.val, CMD_PWD_STR) == 0) {
        return CMD_PWD;
    }
    if (strcmp(cmd_in.val, CMD_JOIN_STR) == 0) {
        return CMD_JOIN;
    }
    if (strcmp(cmd_in.val, CMD_TALK_STR) == 0) {
        return CMD_TALK;
    }

    return CMD_PARSE_ERROR;
}

command_t
parse(string in) {
    tok_t tok = str_tok_init(COMMAND_DELIM, in);
    string command_str = str_tok(&tok, 0);

    command_code_t code = parse_command(command_str);
    if (code != CMD_PARSE_ERROR && tok.has_more) {
        // only when there is a leftover
        string sub = str_sub(in, tok.start, -1);
        str_destroy(sub);
        str_destroy(command_str);
        return command_new(code, str_trim(sub));

    }

    str_destroy(command_str);
    return command_new(CMD_PARSE_ERROR, NIL);
}
