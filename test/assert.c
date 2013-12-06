#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"

static void fail(test_t*, char*, char*, int);

test_t* test(test_func f) {
    test_t *t = malloc(sizeof(test_t));

    t->t = f;
    t->failed = 0;
    t->msg = NULL;

    return t;
}

void eval(test_t *t) {
    if (t->failed) {
        printf("F");
    } else {
        printf(".");
    }
}

void print_stats(test_t **tests, int count) {
    printf("\n\n");
    int failed_count = 0;
    for (int i = 0; i < count && tests[i] != NULL; ++i)
    {
        test_t *test = tests[i];

        if (test->failed) {
            ++failed_count;
            printf("Test failed in %s:%d\n%s\n", test->file, test->line, test->msg);
        }
    }

    if (failed_count > 0) {
        printf("%d test(s) failed.\n", failed_count);
    } else {
        printf("All tests passed.\n");
    }
}

void _assert(test_t *t, char *msg, int expr, char *file, int line) {
    if (!expr) {
        fail(t, msg, file, line);
    }
}

void _assert_eq_str(test_t *t, char* exp, char* act, char *file, int line) {
    // TODO: this is not such a good idea. figure out the exact length
    char msg[1000];
    sprintf(msg,
            "String assertion failed.\n\texpected: \"%s\"\n\tactual: \"%s\"",
            exp, act);
    _assert(t, msg, strcmp(exp, act) == 0, file, line);
}

void _assert_eq_int(test_t *t, int exp, int act, char *file, int line) {
    char msg[1000];
    sprintf(msg,
            "Integer assertion failed.\n\texpected: %d\n\tactual: %d",
            exp, act);
    _assert(t, msg, exp == act, file, line);
}

void _assert_neq_int(test_t *t, int exp, int act, char *file, int line) {
    char msg[1000];
    sprintf(msg,
            "Integer assertion failed.\n\texpected %d not be %d.",
            exp, act);
    _assert(t, msg, exp != act, file, line);
}

static void
fail(test_t *t, char *msg, char *file, int line) {
    t->msg = msg;
    t->file = file;
    t->line = line;
    t->failed = 1;
}