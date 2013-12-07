#include <stdlib.h>
#include "assert.h"

#include <string.h>
#include "../src/string.h"

#define TEST_COUNT 100

void string_create(test_t *t) {
    string str = str_new("hello");

    assert_eq_int(t, 5, str.len);
    assert_eq_int(t, 5, strlen(str.val));

    str_destroy(str);
}

void string_empty(test_t *t) {
    string str = str_empty();

    assert_eq_int(t, 0, str.len);
    assert_eq_str(t, "", str.val);
    str_destroy(str);
}

void string_is_empty(test_t *t) {
    string str = str_empty();

    assert(t, "String is not empty.", str_is_empty(str));
    str_destroy(str);
}

void sub_string_when_end_equal_start(test_t *t) {
    string str = str_new("test");
    string sub = str_sub(str, 1, 0);

    assert(t, "String is not empty.", str_is_empty(sub));
    str_destroy(str);
    str_destroy(sub);
}

void sub_string(test_t *t) {
    string str = str_new("test");
    string sub = str_sub(str, 0, 2);

    assert_eq_str(t, "te", sub.val);

    str_destroy(str);
    str_destroy(sub);
}

void sub_on_error_returns_nil(test_t *t) {
    string str = str_new("test");
    // start is out of bound
    string sub1 = str_sub(str, 100, 0);
    string sub2 = str_sub(str, -1, 0);

    // wanting to read more than allowed
    string sub3 = str_sub(str, 0, 100);

    assert(t, "Out of bound did not return nil.", str_eq(sub1, NIL));
    assert(t, "Out of bound did not return nil.", str_eq(sub2, NIL));
    assert(t, "Overreading did not reutrn nil", str_eq(sub3, NIL));
}

void substring_return_rest(test_t *t) {
    string str = new_str("test");

    string s1 = str_sub(str, 0, -1);
    string s2 = str_sub(str, 1, -1);

    assert_str_eq(t, "test", s1.val);
    assert_str_eq(t, "est", s2.val);

    str_destroy(str);
    str_destroy(t1);
    str_destroy(t2);
}

void string_equality(test_t *t) {
    string s1 = str_new("test");
    string s2 = str_new("test");

    assert(t, "Empty does not equal empty.", str_eq(str_empty(), str_empty()));
    assert(t, "Nil does not equal nil", str_eq(str_nil(), str_nil()));
    assert(t, "'test' does not equal 'test'", str_eq(s1, s2));

    str_destroy(s1);
    str_destroy(s2);
}

void string_nil_not_eq_empty(test_t *t) {
    assert(t, "Empty and nil string does equal.", str_eq(str_nil(), str_empty()) == 0);
}

void tokenizer_find_first_without_sep(test_t *t) {
    string str = str_new("hello world");
    tok_t tok = str_tok_init(' ', str);

    string t1 = str_tok(&tok, 0);
    assert_eq_str(t, "hello", t1.val);

    str_destroy(str);
    str_destroy(t1);
}

void tokienizer_find_first_with_sep(test_t *t) {
    string str = str_new("hello world");
    tok_t tok = str_tok_init(' ', str);

    string t1 = str_tok(&tok, 1);

    assert_eq_str(t, "hello ", t1.val);
    assert(t, "Does not have more tokens.", tok.has_more);

    str_destroy(str);
    str_destroy(t1);
}

void tokenizer_returns_rest_of_string_when_no_more_sep(test_t *t) {
    string str = str_new("hello world");
    tok_t tok = str_tok_init(' ', str);

    str_destroy(str_tok(&tok, 0));
    string t1 = str_tok(&tok, 0);

    assert_eq_str(t, "world", t1.val);
    assert_not(t, "Has more tokens", tok.has_more);

    str_destroy(str);
    str_destroy(t1);
}

int main()
{
    test_t *tests[TEST_COUNT] = {
        test(string_create),
        test(string_empty),
        test(sub_string_when_end_equal_start),
        test(string_is_empty),
        test(sub_string),
        test(string_equality),
        test(string_nil_not_eq_empty),
        test(sub_on_error_returns_nil),
        test(tokenizer_find_first_without_sep),
        test(tokienizer_find_first_with_sep),
        test(tokenizer_returns_rest_of_string_when_no_more_sep),
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
