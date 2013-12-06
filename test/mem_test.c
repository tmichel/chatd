#include <stdlib.h>
#include "lib/assert.h"
#include "lib/tests.h"
#include "../src/user.h"
#include "../src/mem.h"


void test_store_user() {
    mem_init();

    user_t* user = new_user();
    mem_res res = mem_store_user(user);

    assert_eq_int(res, MEM_OK);

    free_user(user);
}

void test_lookup_existing_user() {
    mem_init();
    user_t *user = new_user_with_name("test");
    mem_store_user(user);

    user_t *found = NULL;

    mem_res res = mem_lookup_user("test", &found);

    assert_eq_int(MEM_OK, res);
    assert_eq_str("test", found->username);

    free_user(user);
}

void test_lookup_non_exsisting_user() {
    mem_init();
    user_t *user = new_user_with_name("test");
    mem_store_user(user);

    user_t *found = NULL;

    mem_res res = mem_lookup_user("test-non", &found);

    assert_eq_int(MEM_NOTFOUND, res);
    assert("'found' is not nil", NULL == found);

    free_user(user);
}