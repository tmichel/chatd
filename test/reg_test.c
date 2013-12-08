#include <stdlib.h>
#include <string.h>
#include "assert.h"

#include "../src/command.h"
#include "../src/mem.h"
#include "../src/const.h"

#define TEST_COUNT 100

void test_user_reg_without_password(test_t *t) {
    mem_init();
    command_t cmd = command_new(CMD_REG, str_new("test"));

    cr_t res = command_execute(cmd, NULL);
    assert_eq_int(t,CMD_RES_OK, res.code);

    user_t* u = NULL;
    mem_res r = mem_lookup_user("test", &u);
    assert_eq_int(t, MEM_OK, r);

    command_destroy(cmd);
}

void test_user_reg_with_without_args(test_t *t) {
    mem_init();
    command_t cmd = command_new(CMD_REG, NIL);

    cr_t res = command_execute(cmd, NULL);

    assert_eq_int(t, CMD_RES_PARSE_ERR, res.code);

    command_destroy(cmd);
}

void test_user_reg_twice(test_t *t) {
    mem_init();
    command_t cmd1 = command_new(CMD_REG, str_new("test"));
    command_t cmd2 = command_new(CMD_REG, str_new("test"));

    cr_t res1 = command_execute(cmd1, NULL);
    cr_t res2 = command_execute(cmd2, NULL);

    assert_eq_int(t, CMD_RES_USR_DUP, res2.code);

    free(res1.user);
    command_destroy(cmd1);
    command_destroy(cmd2);
}

int main() {
    test_t *tests[TEST_COUNT] = {
        test(test_user_reg_without_password),
        test(test_user_reg_with_without_args),
        test(test_user_reg_twice),
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
