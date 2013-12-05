#include <stdlib.h>
#include <string.h>

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

    char *text = "REG test pass";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert("command code is not CMD_REG", CMD_REG == cmd->code);
    assert_eq_str("test pass", cmd->args);

    teardown();
}

void test_parse_error() {
    setup();

    char *text = "NOT hello world";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert("Command code is not CMD_PARSE_ERROR", CMD_PARSE_ERROR == cmd->code);
    assert("Command arguments is not nil", NULL == cmd->args);

    teardown();
}

void test_parse_exit() {
    setup();

    char *text = "EXIT";
    size_t len = strlen(text);
    parse(text, len, cmd);

    assert("Command code is not CMD_EXIT", CMD_EXIT == cmd->code);
    assert("Command arguments is not nil", NULL == cmd->args);
    teardown();
}

void test_parse_exit_with_line_feed() {
    setup();

    char *text = "EXIT\n";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert("Command code is not CMD_EXIT", CMD_EXIT == cmd->code);
    assert("Command arguments is not nil", NULL == cmd->args);
    teardown();
}

void test_parse_exit_with_line_ending() {
    setup();

    char *text = "EXIT\r\n";
    size_t len = strlen(text);
    parse(text, len, cmd);

    assert("Command code is not CMD_EXIT", CMD_EXIT == cmd->code);

    teardown();
}

void test_parse_pwd() {
    setup();

    char *text = "PWD new old";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert("command code is not CMD_PWD", CMD_PWD == cmd->code);
    assert_eq_str("new old", cmd->args);

    teardown();
}