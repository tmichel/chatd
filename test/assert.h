#ifndef _ASSERT_H
#define _ASSERT_H

#define assert(t, msg, expr) _assert(t, msg, expr, __FILE__, __LINE__)
#define assert_eq_str(t, exp, act) _assert_eq_str(t, exp, act, __FILE__, __LINE__)
#define assert_eq_int(t, exp, act) _assert_eq_int(t, exp, act, __FILE__, __LINE__)
#define assert_neq_int(t, exp, act) _assert_neq_int(t, exp, act, __FILE__, __LINE__)

struct __test_t {
    int failed;
    char *msg;
    char *file;
    int line;
    void (*t)(struct __test_t*);
};

typedef struct __test_t test_t;

typedef void (*test_func)(test_t*);

test_t *test(test_func);

void print_stats(test_t**, int);

void eval(test_t*);

void _assert(test_t *t, char *msg, int expr, char *file, int line);

void _assert_eq_str(test_t *t, char* exp, char* act, char *file, int line);

void _assert_eq_int(test_t *t, int exp, int act, char *file, int line);

void _assert_neq_int(test_t *t, int exp, int act, char *file, int line);

#endif
