#include <string.h>
#include <stdlib.h>

#include "const.h"
#include "parser.h"
#include "command.h"

static cc_t
parse_command(string cmd_in) {
    string s = str_trim(cmd_in);
    int result = CMD_PARSE_ERROR;

    if (strcmp(s.val, CMD_REG_STR) == 0) {
        result = CMD_REG;
    } else if (strcmp(s.val, CMD_EXIT_STR) == 0) {
        result = CMD_EXIT;
    } else if (strcmp(s.val, CMD_PWD_STR) == 0) {
        result = CMD_PWD;
    } else if (strcmp(s.val, CMD_JOIN_STR) == 0) {
        result = CMD_JOIN;
    } else if (strcmp(s.val, CMD_TALK_STR) == 0) {
        result = CMD_TALK;
    } else if (strcmp(s.val, CMD_LEAVE_STR) == 0) {
        result = CMD_LEAVE;
    }

    str_destroy(s);
    return result;
}

command_t
parse(string in) {
    tok_t tok = str_tok_init(COMMAND_DELIM, in);
    string command_str = str_tok(&tok, SEP_EXCL);
    cc_t code = parse_command(command_str);

    if (code == CMD_PARSE_ERROR) {
        str_destroy(command_str);
        return command_new(CMD_PARSE_ERROR, NIL);
    }

    if (tok.has_more) {
        // only when there is a leftover
        string sub = str_tok_rest(tok);
        string trim = str_trim(sub);
        str_destroy(sub);
        str_destroy(command_str);

        return command_new(code, trim);
    }

    str_destroy(command_str);
    return command_new(code, NIL);
}
