#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"

#include "../src/command.h"
#include "../src/mem.h"
#include "../src/room.h"
#include "../src/const.h"

#define TEST_COUNT 100

void test_join_existing_room(test_t *t) {
    mem_init();
    user_t *u = user_new();
    room_t *r = room_new("test");
    mem_store_room(r);

    command_t c = command_new(CMD_JOIN, str_new("test"));

    cr_t res = command_execute(c, u);

    assert_eq_int(t, CMD_RES_OK, res.code);
    assert_eq_int(t, 0, r->admins->len);
}

void test_join_new_room(test_t *t) {
    mem_init();
    user_t *u = user_new();

    command_t c = command_new(CMD_JOIN, str_new("test"));

    cr_t res = command_execute(c, u);

    assert_eq_int(t, CMD_RES_OK, res.code);

    room_t *r;
    mem_lookup_room("test", &r);
    assert_eq_int(t, 1, r->admins->len);
}

int main()
{
    test_t *tests[TEST_COUNT] = { NULL };
    tests[0] = test(test_join_existing_room);
    tests[1] = test(test_join_new_room);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}