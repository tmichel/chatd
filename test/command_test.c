#include "assert.h"

#include "../src/user.h"
#include "../src/user.h"
#include "../src/command.h"
#include "../src/mem.h"

#include <stdlib.h>
#include <string.h>

void test_user_reg_without_password(test_t *t) {
    mem_init();
    command_t cmd;
    cmd.code = CMD_REG;
    cmd.args = calloc(sizeof(char), sizeof(char) * 5);
    strcpy(cmd.args, "test");

    command_result_t res = user_reg(&cmd);
    assert_eq_int(t,CMD_RES_OK, res.code);

    user_t* u = NULL;
    mem_res r = mem_lookup_user("test", &u);
    assert_eq_int(t,MEM_OK, r);

    free(res.msg);
    free(cmd.args);
}

void test_user_reg_with_without_args(test_t *t) {
    mem_init();
    command_t cmd;
    cmd.code = CMD_REG;
    cmd.args = calloc(sizeof(char), sizeof(char));

    command_result_t res = user_reg(&cmd);

    assert_eq_int(t,CMD_RES_ERR, res.code);

    free(res.msg);
    free(cmd.args);
}

void test_user_reg_twice(test_t *t) {
    mem_init();
    command_t cmd;
    cmd.code = CMD_REG;
    cmd.args = calloc(sizeof(char), sizeof(char) * 5);
    strcpy(cmd.args, "test");

    command_t cmd2;
    cmd2.code = CMD_REG;
    cmd2.args = calloc(sizeof(char), sizeof(char) * 5);
    strcpy(cmd2.args, "test");

    command_result_t res1 = user_reg(&cmd);
    command_result_t res2 = user_reg(&cmd2);

    assert_eq_int(t,CMD_RES_ERR, res2.code);

    free(res1.msg);
    free(res2.msg);
    free(cmd.args);
    free(cmd2.args);
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = { NULL };

    tests[0] = test(test_user_reg_without_password);
    tests[1] = test(test_user_reg_with_without_args);
    tests[2] = test(test_user_reg_twice);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;

}
