#include <stdlib.h>

#include "lib/assert.h"
#include "lib/tests.h"
#include "../src/parser.h"

command_t *cmd;

static void setup() {
    cmd = new_command();
}

static void teardown() {
    free_command(cmd);
}

void test_parse_reg() {
    setup();

    parse("REG hello world", cmd);
    assert("command code is not CMD_REG", CMD_REG == cmd->code);

    teardown();
}

void test_parse_error() {
    setup();

    parse("NOT hello world", cmd);
    assert("Command code is not CMD_PARSE_ERROR", CMD_PARSE_ERROR == cmd->code);

    teardown();
}

void test_parse_exit() {
    setup();
    parse("EXIT", cmd);
    assert("Command code is not CMD_EXIT", CMD_EXIT == cmd->code);
    assert("Command arguments is not nil", NULL == cmd->args);
    teardown();
}
