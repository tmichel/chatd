#ifndef _ASSERT_H
#define _ASSERT_H

#define assert(msg, expr) _assert(msg, expr, __FILE__, __LINE__)
#define assert_eq_str(exp, act) _assert_eq_str(exp, act, __FILE__, __LINE__)
#define assert_eq_int(exp, act) _assert_eq_int(exp, act, __FILE__, __LINE__)
#define assert_neq_int(exp, act) _assert_neq_int(exp, act, __FILE__, __LINE__)

void _assert(const char *msg, int expr, char *file, int line);

void _assert_eq_str(char* exp, char* act, char *file, int line);

void _assert_eq_int(int exp, int act, char *file, int line);

void _assert_neq_int(int exp, int act, char *file, int line);

void pass();

void fail(const char *msg, char *file, int line);

void done();

#endif
