#include <stdlib.h>

#include "lib/assert.h"
#include "lib/tests.h"
#include "../src/util.h"

void test_empty_string() {
    char dest[10];
    empty(dest, 10);

    for (int i = 0; i < 10; ++i)
    {
        assert("Part of the string is not 0", dest[i] == 0);
    }
}

void test_trim_leading_whitespace() {
    char *str = "\t      \nabc";
    char dest[10];
    empty(dest, 10);

    trim(str, dest);

    assert_eq_str("abc", dest);
}

void test_trim_trailing_whitesapces() {
    char *str = "abc\t     \n";
    char dest[10];
    empty(dest, 10);

    trim(str, dest);
    assert_eq_str("abc", dest);
}

void test_trim_leading_and_trailing_whitesapces() {
    char *str = "   \t    \nabc\t     \n";
    char *dest = calloc(sizeof(char), sizeof(char) * 10);

    trim(str, dest);

    assert_eq_str("abc", dest);
    free(dest);
}
