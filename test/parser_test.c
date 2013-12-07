#include <stdlib.h>
#include <string.h>

#include "assert.h"

#include "../src/parser.h"

void test_parse_reg(test_t *t) {
    command_t cmd = parse(str_new("REG test pass"));
    assert(t,"command code is not CMD_REG", CMD_REG == cmd.code);
    assert_eq_str(t, "test pass", cmd.args.val);
}

void test_parse_reg_without_args(test_t *t) {
    char text[4] = "REG\n";
    command_t cmd = parse(str_newn(text, 4));
    assert_eq_int(t, CMD_REG, cmd.code);
}

void test_parse_error(test_t *t) {
    command_t cmd = parse(str_new("NOT hello world"));
    assert(t,"Command code is not CMD_PARSE_ERROR", CMD_PARSE_ERROR == cmd.code);
    assert(t,"Command arguments is not nil", str_is_nil(cmd.args));
}

void test_parse_exit(test_t *t) {
    command_t cmd = parse(str_new("EXIT"));
    assert(t,"Command code is not CMD_EXIT", CMD_EXIT == cmd.code);
    assert(t,"Command arguments is not nil", str_is_nil(cmd.args));
}

void test_parse_exit_with_line_feed(test_t *t) {
    command_t cmd = parse(str_new("EXIT\n"));
    assert(t,"Command code is not CMD_EXIT", CMD_EXIT == cmd.code);
    assert(t,"Command arguments is not nil", str_is_nil(cmd.args));
}

void test_parse_exit_with_line_ending(test_t *t) {
    command_t cmd = parse(str_new("EXIT\r\n"));
    assert(t,"Command code is not CMD_EXIT", CMD_EXIT == cmd.code);
}

void test_parse_pwd(test_t *t) {
    command_t cmd = parse(str_new("PWD new old"));
    assert(t,"command code is not CMD_PWD", CMD_PWD == cmd.code);
    assert_eq_str(t, "new old", cmd.args.val);
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = {
        test(test_parse_reg),
        test(test_parse_reg_without_args),
        test(test_parse_error),
        test(test_parse_exit),
        test(test_parse_exit_with_line_feed),
        test(test_parse_exit_with_line_ending),
        test(test_parse_pwd),
    };

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
