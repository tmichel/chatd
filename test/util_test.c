#include <stdlib.h>
#include "assert.h"
#include "../src/util.h"

void test_empty_string(test_t *t) {
    char dest[10];
    empty(dest, 10);

    for (int i = 0; i < 10; ++i)
    {
        assert(t,"Part of the string is not 0", dest[i] == 0);
    }
}

void test_trim_leading_whitespace(test_t *t) {
    char *str = "\t      \nabc";
    char dest[10];
    empty(dest, 10);

    trim(str, dest);

    assert_eq_str(t,"abc", dest);
}

void test_trim_trailing_whitesapces(test_t *t) {
    char *str = "abc\t     \n";
    char dest[10];
    empty(dest, 10);

    trim(str, dest);
    assert_eq_str(t,"abc", dest);
}

void test_trim_leading_and_trailing_whitesapces(test_t *t) {
    char *str = "   \t    \nabc\t     \n";
    char *dest = calloc(sizeof(char), sizeof(char) * 10);

    trim(str, dest);

    assert_eq_str(t,"abc", dest);
    free(dest);
}

#define TEST_COUNT 100

int main() {
        test_t *tests[TEST_COUNT] = { NULL };

    tests[0] = test(test_empty_string);
    tests[1] = test(test_trim_leading_whitespace);
    tests[2] = test(test_trim_trailing_whitesapces);
    tests[2] = test(test_trim_leading_and_trailing_whitesapces);

    for (int i = 0; i < TEST_COUNT && tests[i] != NULL; ++i)
    {
        test_t *t = tests[i];
        t->t(t);
        eval(t);
    }

    print_stats(tests, TEST_COUNT);
    return 0;
}
