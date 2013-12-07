#include "assert.h"

#include "../src/user.h"
#include "../src/command.h"
#include "../src/mem.h"

#include <stdlib.h>
#include <string.h>

void test_execute_command_with_parse_error(test_t *t) {
    command_t c = command_new(CMD_PARSE_ERROR, NIL);
    command_result_t res = command_execute(c, NULL);

    assert_eq_int(t, CMD_RES_ERR, res.code);
}

void test_execute_command_without_user(test_t *t) {
    command_t c = command_new(CMD_TALK, NIL);
    command_result_t res = command_execute(c, NULL);

    assert_eq_int(t, CMD_RES_NO_USR, res.code);
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = {
        test(test_execute_command_without_user),
        test(test_execute_command_with_parse_error),
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
