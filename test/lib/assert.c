#include <stdio.h>
#include <stdlib.h>

#include "assert.h"

int passed_asserts;

void _assert(const char *msg, int expr, char *file, int line) {
    if (!expr) {
        fail(msg, file, line);
    }
    pass();
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
