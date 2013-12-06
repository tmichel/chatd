#include <stdlib.h>
#include <string.h>

#include "assert.h"

#include "../src/parser.h"

command_t *cmd;

static void setup() {
    cmd = new_command();
}

static void teardown() {
    free_command(cmd);
}

void test_parse_reg(test_t *t) {
    setup();

    char *text = "REG test pass";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert(t,"command code is not CMD_REG", CMD_REG == cmd->code);
    assert_eq_str(t,"test pass", cmd->args);

    teardown();
}

void test_parse_reg_without_args(test_t *t) {
    setup();

    char text[4] = "REG\n";
    // size_t len = strlen(text);
    parse(text, 4, cmd);
    assert_eq_int(t,CMD_REG, cmd->code);

    teardown();
}

void test_parse_error(test_t *t) {
    setup();

    char *text = "NOT hello world";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert(t,"Command code is not CMD_PARSE_ERROR", CMD_PARSE_ERROR == cmd->code);
    assert(t,"Command arguments is not nil", NULL == cmd->args);

    teardown();
}

void test_parse_exit(test_t *t) {
    setup();

    char *text = "EXIT";
    size_t len = strlen(text);
    parse(text, len, cmd);

    assert(t,"Command code is not CMD_EXIT", CMD_EXIT == cmd->code);
    assert(t,"Command arguments is not nil", NULL == cmd->args);
    teardown();
}

void test_parse_exit_with_line_feed(test_t *t) {
    setup();

    char *text = "EXIT\n";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert(t,"Command code is not CMD_EXIT", CMD_EXIT == cmd->code);
    assert(t,"Command arguments is not nil", NULL == cmd->args);
    teardown();
}

void test_parse_exit_with_line_ending(test_t *t) {
    setup();

    char *text = "EXIT\r\n";
    size_t len = strlen(text);
    parse(text, len, cmd);

    assert(t,"Command code is not CMD_EXIT", CMD_EXIT == cmd->code);

    teardown();
}

void test_parse_pwd(test_t *t) {
    setup();

    char *text = "PWD new old";
    size_t len = strlen(text);
    parse(text, len, cmd);
    assert(t,"command code is not CMD_PWD", CMD_PWD == cmd->code);
    assert_eq_str(t,"new old", cmd->args);

    teardown();
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = { NULL };

    tests[0] = test(test_parse_reg);
    tests[1] = test(test_parse_reg_without_args);
    tests[2] = test(test_parse_error);
    tests[3] = test(test_parse_exit);
    tests[4] = test(test_parse_exit_with_line_feed);
    tests[5] = test(test_parse_exit_with_line_ending);
    tests[6] = test(test_parse_pwd);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
