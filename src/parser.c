#include <string.h>
#include <stdlib.h>

#include "const.h"
#include "parser.h"
#include "command.h"

#define CMD_NR 8

char *CMD[CMD_NR] = {
    /* 0 */ "REG",
    /* 1 */ "EXIT",
    /* 2 */ "PWD",
    /* 3 */ "JOIN",
    /* 4 */ "TALK",
    /* 5 */ "LEAVE",
    /* 6 */ "MSG",
    /* 7 */ "GRANT",
};

static cc_t
parse_command(string cmd_in) {
    string s = str_trim(cmd_in);
    cc_t code = CMD_PARSE_ERROR;

    for (cc_t c = 0; c < CMD_NR; ++c) {
        if (strcmp(s.val, CMD[c]) == 0) {
            code = c;
        }
    }

    str_destroy(s);
    return code;
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
