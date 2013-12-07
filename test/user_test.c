#include <stdlib.h>
#include "assert.h"

#include "../src/user.h"

void test_create_new_user_with_name(test_t *t) {
    user_t *user = NULL;
    user = user_new_with_name("test");

    assert(t,"User is nil.", NULL != user);
    assert_eq_str(t,"test", user->username);

    free_user(user);
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = { NULL };

    tests[0] = test(test_create_new_user_with_name);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
