#include <stdlib.h>
#include "assert.h"

#define TEST_COUNT 100

#include "../src/room.h"
#include "../src/user.h"
#include "../src/command.h"
#include "../src/const.h"

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

void remove_user_from_room_when_exiting(test_t *t) {
    room_t *room = room_new("room");
    user_t *user = user_new_with_name("user");
    user->sock = 1;
    vec_add(room->users, user);
    vec_add(user->rooms, room);

    room_remove_user(room, user);

    assert_not(t, "User was not removed from the room.", vec_contains(room->users, user));
    assert_not(t, "Room was not removed from user's room list.", vec_contains(user->rooms, room));
    user_free(user);
    room_free(room);
}

void remove_user_from_room(test_t *t) {
    room_t *room = room_new("room");
    user_t *user = user_new_with_name("user");
    user->sock = 1;
    vec_add(room->users, user);
    vec_add(user->rooms, room);

    room_remove_user(room, user);

    assert_not(t, "User was not removed from the room.", vec_contains(room->users, user));
    assert_not(t, "Room was not removed from user's room list.", vec_contains(user->rooms, room));
    user_free(user);
    room_free(room);

}

void grant_admin_rights(test_t *t) {
    room_t *room = room_new("room");
    user_t *admin = user_new_with_name("admin");
    user_t *user = user_new_with_name("user");
    vec_add(room->users, user);
    vec_add(room->admins, admin);

    room_admin(room, admin, str_new(user->username));

    assert(t, "User was not granted admin rights.", vec_contains(room->admins, user));

    user_free(admin);
    user_free(user);
    room_free(room);
}

void trying_to_grant_admin_right_without_being_an_admin(test_t *t) {
    room_t *room = room_new("room");
    user_t *not_admin = user_new_with_name("not_admin");
    user_t *user = user_new_with_name("user");

    cr_t res = room_admin(room, not_admin, str_new(user->username));

    assert_eq_int(t, CMD_RES_NOT_ALLOWED, res.code);

    user_free(not_admin);
    user_free(user);
    room_free(room);
}

void trying_to_grand_admin_right_to_user_not_in_the_room(test_t *t) {
    room_t *room = room_new("room");
    user_t *admin = user_new_with_name("admin");
    user_t *user = user_new_with_name("user");
    vec_add(room->admins, admin);

    cr_t res = room_admin(room, admin, str_new(user->username));

    assert_eq_int(t, CMD_RES_NO_USR, res.code);

    user_free(admin);
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
        test(remove_user_from_room_when_exiting),
        test(remove_user_from_room),
        test(grant_admin_rights),
        test(trying_to_grant_admin_right_without_being_an_admin),
        test(trying_to_grand_admin_right_to_user_not_in_the_room),
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
