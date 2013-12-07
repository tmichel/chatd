#include <stdlib.h>
#include "assert.h"

#include "../src/mem.h"
#include "../src/command.h"

#define TEST_COUNT 100

void talk_in_a_non_existent_room(test_t *t) {
    mem_init();

    user_t *user = user_new();
    command_t cmd = command_new(CMD_TALK, str_new("non-existent"));
    cr_t res = command_execute(cmd, user);

    assert_eq_int(t, CMD_RES_NO_ROOM, res.code);
}

void talk_no_args(test_t *t) {
    mem_init();

    user_t *user = user_new();
    command_t cmd = command_new(CMD_TALK, str_nil());
    cr_t res = command_execute(cmd, user);

    assert_eq_int(t, CMD_RES_ERR, res.code);
}

int main() {
    test_t *tests[TEST_COUNT] = {
        test(talk_in_a_non_existent_room),
        test(talk_no_args),
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
