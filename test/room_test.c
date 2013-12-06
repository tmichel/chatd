#include <stdlib.h>
#include "assert.h"

#define TEST_COUNT 100

#include "../src/room.h"

void test_add_user_to_room(test_t *t) {
    user_t *u = new_user();
    room_t *r = room_new("test");

    room_add_user(r, u, 0);

    assert_eq_int(t, 1, r->users->len);
    assert_eq_int(t, 0, r->admins->len);
}

void test_add_user_to_room_as_admin(test_t *t) {
    user_t *u = new_user();
    room_t *r = room_new("test");

    room_add_user(r, u, 1);

    assert_eq_int(t, 1, r->admins->len);
}

int main()
{
    test_t *tests[TEST_COUNT] = { NULL };
    tests[0] = test(test_add_user_to_room);
    tests[1] = test(test_add_user_to_room_as_admin);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
