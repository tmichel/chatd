#include "assert.h"

#include "../src/user.h"
#include "../src/command.h"
#include "../src/mem.h"

#include <stdlib.h>
#include <string.h>



void test_execute_command_without_user(test_t *t) {
    command_t *c = new_command();
    c->code = CMD_TALK;
    command_result_t res = command_execute(NULL, c);

    assert_eq_int(t, CMD_RES_NO_USR, res.code);
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = { NULL };

    tests[0] = test(test_execute_command_without_user);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
