#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"

int passed_asserts;

void _assert(const char *msg, int expr, char *file, int line) {
    if (!expr) {
        fail(msg, file, line);
    }
    pass();
}

void _assert_eq_str(char* exp, char* act, char *file, int line) {
    // TODO: this is not such a good idea. figure out the exact length
    char msg[1000];
    sprintf(msg,
            "String assertion failed.\n\texpected: \"%s\"\n\tactual: \"%s\"",
            exp, act);
    _assert(msg, strcmp(exp, act) == 0, file, line);
}

void pass() {
    printf(".");
    ++passed_asserts;
}

void fail(const char *msg, char *file, int line) {
    printf("F\nin %s:%d %s\n", file, line, msg);
    printf("Number of asserts: %d\n", passed_asserts+1);
    exit(EXIT_SUCCESS);
}

void done() {
    printf("\n");
    printf("Every assert passed.\n");
    printf("Number of asserts: %d\n", passed_asserts);
}
