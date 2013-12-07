#include <stdlib.h>
#include "assert.h"

#include "../src/user.h"
#include "../src/mem.h"


void test_store_user(test_t *t) {
    mem_init();

    user_t* user = user_new();
    mem_res res = mem_store_user(user);

    assert_eq_int(t,res, MEM_OK);

    user_free(user);
}

void test_lookup_existing_user(test_t *t) {
    mem_init();
    user_t *user = user_new_with_name("test");
    mem_store_user(user);

    user_t *found = NULL;

    mem_res res = mem_lookup_user("test", &found);

    assert_eq_int(t,MEM_OK, res);
    assert_eq_str(t,"test", found->username);

    user_free(user);
}

void test_lookup_non_exsisting_user(test_t *t) {
    mem_init();
    user_t *user = user_new_with_name("test");
    mem_store_user(user);

    user_t *found = NULL;

    mem_res res = mem_lookup_user("test-non", &found);

    assert_eq_int(t,MEM_NOTFOUND, res);
    assert(t,"'found' is not nil", NULL == found);

    user_free(user);
}

#define TEST_COUNT 100

int main() {
    test_t *tests[TEST_COUNT] = { NULL };

    tests[0] = test(test_store_user);
    tests[1] = test(test_lookup_existing_user);
    tests[2] = test(test_lookup_non_exsisting_user);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
