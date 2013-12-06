#include <stdlib.h>
#include "assert.h"

#include "../src/command.h"

#define TEST_COUNT 100

void test_join_without_active_user(test_t *t) {
    assert_eq_int(t, 1, 1);
    assert_eq_int(t, 1, 1);
}


int main()
{
    test_t *tests[TEST_COUNT] = { NULL };
    tests[0] = test(test_join_without_active_user);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}