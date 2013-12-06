#include <stdlib.h>
#include "assert.h"

#include "../src/user.h"

void test_create_new_user_with_name(test_t *t) {
    user_t *user = NULL;
    user = new_user_with_name("test");

    assert(t,"User is nil.", NULL != user);
    assert_eq_str(t,"test", user->username);

    free_user(user);
}

void test_new_user_gets_token(test_t *t) {
    user_t *user = NULL;
    user = new_user();

    assert(t,"Token is nil.", user->token != NULL);

    free_user(user);
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = { NULL };

    tests[0] = test(test_create_new_user_with_name);
    tests[1] = test(test_new_user_gets_token);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
