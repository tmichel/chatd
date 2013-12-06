#include <stdlib.h>
#include "lib/assert.h"
#include "lib/tests.h"
#include "../src/user.h"

void test_create_new_user_with_name() {
    user_t *user = NULL;
    user = new_user_with_name("test");

    assert("User is nil.", NULL != user);
    assert_eq_str("test", user->username);

    free_user(user);
}

void test_new_user_gets_token() {
    user_t *user = NULL;
    user = new_user();

    assert("Token is nil.", user->token != NULL);

    free_user(user);
}