#include <stdlib.h>
#include "assert.h"

#define TEST_COUNT 100

#include "../src/room.h"
#include "../src/command.h"

void add_user_to_room(test_t *t) {
    user_t *u = user_new();
    room_t *r = room_new("test");

    room_add_user(r, u, 0);

    assert_eq_int(t, 1, r->users->len);
    assert_eq_int(t, 0, r->admins->len);

    user_free(u);
    room_free(r);
}

void add_user_to_room_as_admin(test_t *t) {
    user_t *u = user_new();
    room_t *r = room_new("test");

    room_add_user(r, u, 1);

    assert_eq_int(t, 1, r->admins->len);
    user_free(u);
    room_free(r);
}

void talk_in_room_when_user_is_not_in(test_t *t) {
    room_t *room = room_new("test");
    user_t *user = user_new();

    cr_t res = room_send_msg(room, user, str_new("hello"));

    assert_eq_int(t, CMD_RES_NO_USR, res.code);
    user_free(user);
    room_free(room);
}

void talk_in_room_where_banned(test_t *t) {
    room_t *room = room_new("test");
    user_t *user = user_new();

    vec_add(room->banned, user);

    cr_t res = room_send_msg(room, user, str_new("hello"));

    assert_eq_int(t, CMD_RES_USR_BND, res.code);
    user_free(user);
    room_free(room);
}

int main()
{
    test_t *tests[TEST_COUNT] = {
        test(add_user_to_room),
        test(add_user_to_room_as_admin),
        test(talk_in_room_when_user_is_not_in),
        test(talk_in_room_where_banned),
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
